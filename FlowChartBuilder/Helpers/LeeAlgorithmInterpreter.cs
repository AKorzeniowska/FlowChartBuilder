using FlowChartBuilder.Models;
using FlowChartBuilder.Providers;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Threading;

namespace FlowChartBuilder.Helpers
{
    public class LeeAlgorithmInterpreter
    {
        static int[][] GetMazeArray(int[,] maze, int mazeHeight, int mazeWidth)
        {
            int[][] convertedMaze = new int[mazeHeight][];
            for (int i = 0; i < mazeHeight; i++)
            {
                var row = new int[mazeWidth];
                for (int x = 0; x < mazeWidth; x++)
                {
                    switch (maze[i, x])
                    {
                        case 0:
                            row[x] = 0;
                            break;
                        case 1:
                            row[x] = 1;
                            break;
                        case 2:
                            row[x] = -3;
                            break;
                        default:
                            row[x] = -1;
                            break;
                    }
                }
                convertedMaze[i] = row;
            }
            Display(convertedMaze);
            return convertedMaze;
        }

        static int[][] GetReversedMazeArray(int[,] maze, int mazeHeight, int mazeWidth)
        {
            int[][] convertedMaze = new int[mazeHeight][];
            for (int i = 0; i < mazeHeight; i++)
            {
                var row = new int[mazeWidth];
                for (int x = 0; x < mazeWidth; x++)
                {
                    switch (maze[i, x])
                    {
                        case 0:
                            row[x] = 0;
                            break;
                        case 1:
                            row[x] = -3;
                            break;
                        case 2:
                            row[x] = 1;
                            break;
                        default:
                            row[x] = -1;
                            break;
                    }
                }
                convertedMaze[i] = row;
            }

            //Display(convertedMaze);
            return convertedMaze;
        }

        static bool IsValidPos(int[][] array, int row, int newRow, int newColumn)
        {
            if (newRow < 0) return false;
            if (newColumn < 0) return false;
            if (newRow >= array.Length) return false;
            if (newColumn >= array[row].Length) return false;
            return true;
        }

        int Move(int[][] arrayTemp,
            int rowIndex,
            int columnIndex,
            int count,
            int turnsCount,
            LineModel visited,
            int[][] Moves,
            int maxMoves,
            int optTurns,
            int[] lastMove,
            bool isReversed,
            ref int lowest,
            ref int leastTurns,
            ref LineModel optimalVisited)
        {

            if (leastTurns <= optTurns)
            {
                return 1;
            }
            if (optimalVisited.GetPointsOfLine().Count <= 3 && optimalVisited.GetPointsOfLine().Count != 0)
            {
                return 1;
            }
            if (count > arrayTemp.Length + arrayTemp[0].Length)
            {
                return 1;
            }
            if (count + 1 >= optimalVisited.GetPointsOfLine().Count && optimalVisited.GetPointsOfLine().Count != 0)
            {
                return 1;
            }
            if (count + 1 > maxMoves)
            {
                return 1;
            }
            if (turnsCount > 5)
            {
                return 1;
            }
            if (count + 1 > lowest)
            {
                return 1;
            }

            if (turnsCount > leastTurns)
            {
                return 1; //hmmmmmmm
            }

            int[][] array = new int[arrayTemp.Length][];
            for (int i = 0; i < arrayTemp.Length; i++)
            {
                var row = arrayTemp[i];
                array[i] = new int[row.Length];
                for (int x = 0; x < row.Length; x++)
                {
                    array[i][x] = row[x];
                }
            }

            int value = array[rowIndex][columnIndex];
            if (value >= 1)
            {
                foreach (var movePair in Moves)
                {
                    int newRow = rowIndex + movePair[0];
                    int newColumn = columnIndex + movePair[1];
                    if (IsValidPos(array, rowIndex, newRow, newColumn))
                    {
                        int testValue = array[newRow][newColumn];
                        if (testValue == 0)
                        {
                            array[newRow][newColumn] = value + 1;
                            // Try another move
                            var visitedDeepCopy = new LineModel(visited);
                            visitedDeepCopy.AddPointToLine(newRow, newColumn);
                            if (lastMove != null && lastMove[0] != movePair[0] && lastMove[1] != movePair[1])
                                Move(array, newRow, newColumn, count + 1, turnsCount + 1, visitedDeepCopy, Moves, maxMoves, optTurns, movePair, isReversed, ref lowest, ref leastTurns, ref optimalVisited);
                            else
                                Move(array, newRow, newColumn, count + 1, turnsCount, visitedDeepCopy, Moves, maxMoves, optTurns, movePair, isReversed, ref lowest, ref leastTurns, ref optimalVisited);
                        }
                        else if (testValue == -3)
                        {
                            if (count + 1 < lowest)
                            {
                                //lock (this)
                                //{
                                lowest = count + 1;
                                var visitedDeepCopy = new LineModel(visited, isReversed);
                                visitedDeepCopy.AddPointToLine(newRow, newColumn);
                                optimalVisited = visitedDeepCopy;
                                if (lastMove != null && lastMove[0] != movePair[0] && lastMove[1] != movePair[1] && turnsCount + 1 < leastTurns)
                                {
                                    leastTurns = turnsCount + 1;
                                }
                                else if (turnsCount < leastTurns)
                                {
                                    leastTurns = turnsCount;
                                }
                                //}
                            }
                            return 1;
                        }
                    }
                }
            }
            return -1;
        }

        public LineModel DoYourJob(int[,] maze, int[][] moves1, int[][] moves2, int maxMoves, int optTurns, int id, int x1, int y1, int x2, int y2)
        {
            Console.WriteLine("NUMEREK " + id);
            var array = GetMazeArray(maze, maze.GetLength(0), maze.GetLength(1));
            var reversedArray = GetReversedMazeArray(maze, maze.GetLength(0), maze.GetLength(1));
            int lowest = int.MaxValue;
            int leastTurns = int.MaxValue;
            var visited = new LineModel();

            Thread t1 = null;
            Thread t2 = null;
            Thread t3 = null;
            Thread t4 = null;

            Stopwatch sw = new Stopwatch();
            sw.Start();
            t1 = new Thread(delegate ()
            {
                Move(array, x1, y1, 0, 0, new LineModel(), moves1, maxMoves, optTurns, null, false, ref leastTurns, ref lowest, ref visited);
            });
            t1.Start();

            t2 = new Thread(delegate ()
            {
                Move(array, x1, y1, 0, 0, new LineModel(), moves2, maxMoves, optTurns, null, false, ref leastTurns, ref lowest, ref visited);
            });
            t2.Start();
            t3 = new Thread(delegate ()
            {
                Move(reversedArray, x2, y2, 0, 0, new LineModel(), MovesProvider.GetReversedMoves(moves1), maxMoves, optTurns, null, true, ref leastTurns, ref lowest, ref visited);
            });
            t3.Start();

            t4 = new Thread(delegate ()
            {
                Move(reversedArray, x2, y2, 0, 0, new LineModel(), MovesProvider.GetReversedMoves(moves2), maxMoves, optTurns, null, true, ref leastTurns, ref lowest, ref visited);
            });
            t4.Start();

            var joinThread1 = new Thread(delegate ()
            {
                t1.Join();
                t2.Join();
                if (lowest == int.MaxValue)
                    lowest = 0;
            });
            joinThread1.Start();

            var joinThread2 = new Thread(delegate ()
            {
                t3.Join();
                t4.Join();
                if (lowest == int.MaxValue)
                    lowest = 0;
            });
            joinThread2.Start();

            //t3.Join();
            //t4.Join();

            //if(lowest == int.MaxValue)
            //    lowest = 0;

            //t1.Join();
            //t2.Join();

            joinThread1.Join();
            joinThread2.Join();

            sw.Stop();
            Console.WriteLine($"Optimal moves: {visited.GetPointsOfLine().Count}");
            Console.WriteLine("ElapsedTime={0}", sw.Elapsed);
            visited.DisplayLine();
            return visited;
        }

        static void Display(int[][] array)
        {
            // Loop over int data and display as characters.
            for (int i = 0; i < array.Length; i++)
            {
                var row = array[i];
                for (int x = 0; x < row.Length; x++)
                {
                    switch (row[x])
                    {
                        case -1:
                            Console.Write('x');
                            break;
                        case 1:
                            Console.Write('1');
                            break;
                        case -3:
                            Console.Write('2');
                            break;
                        case 0:
                            Console.Write(' ');
                            break;
                        default:
                            Console.Write('.');
                            break;
                    }
                }
                // End line.
                Console.WriteLine();
            }
        }

        static void DisplayRawArray(int[][] array)
        {
            for (int i = 0; i < array.Length; i++)
            {
                for (int x = 0; x < array[i].Length; x++)
                {
                    Console.Write(array[i][x]);
                }
                Console.WriteLine();
            }
        }

        static void DisplayVisited(List<KeyValuePair<int, int>> visited)
        {
            foreach (var pair in visited)
            {
                Console.WriteLine(pair.Key + " " + pair.Value);
            }
            Console.WriteLine();
            Console.WriteLine(visited.Count);
            Console.WriteLine();
        }
    }

}
