using FlowChartBuilder.Models;
using FlowChartBuilder.Providers;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

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
                    switch (maze[i,x])
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

        static bool IsValidPos(int[][] array, int row, int newRow, int newColumn)
        {
            if (newRow < 0) return false;
            if (newColumn < 0) return false;
            if (newRow >= array.Length) return false;
            if (newColumn >= array[row].Length) return false;
            return true;
        }

        static int Move(int[][] arrayTemp,
            int rowIndex,
            int columnIndex,
            int count,
            LineModel visited,
            int[][] Moves,
            int maxMoves,
            ref int lowest,
            ref LineModel optimalVisited)
        {

            if (lowest <= 3) return 1;
            if (count > arrayTemp.Length + arrayTemp[0].Length) return 1;
            if (count + 1 > lowest) return 1;
            if (count + 1 > maxMoves) return 1;
            // Copy map so we can modify it and then abandon it.
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
            //    Display(array);
            //    Console.WriteLine();
            //    Console.WriteLine(count);
                // Try all moves.
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
                            // Try another move.
                            var visitedDeepCopy = new LineModel(visited);
                            visitedDeepCopy.AddPointToLine(newRow, newColumn);
                            Move(array, newRow, newColumn, count + 1, visitedDeepCopy, Moves, maxMoves, ref lowest, ref optimalVisited);
                        }
                        else if (testValue == -3)
                        {   
                            if (count + 1 < lowest)
                            {
                                lowest = count + 1;
                                var visitedDeepCopy = new LineModel(visited);
                                visitedDeepCopy.AddPointToLine(newRow, newColumn);
                                optimalVisited = visitedDeepCopy;
                            }
                            return 1;
                        }
                    }
                }
            }
            return -1;
        }

        public static LineModel DoYourJob(int[,] maze, int[][] moves, int maxMoves)
        {
            var array = GetMazeArray(maze, maze.GetLength(0), maze.GetLength(1));
            var visited = new LineModel();
            // Get start position.
            for (int i = 0; i < array.Length; i++)
            {
                var row = array[i];
                for (int x = 0; x < row.Length; x++)
                {
                    
                    // Start square is here.
                    if (row[x] == 1)
                    {
                        int lowest = int.MaxValue;
                        Stopwatch sw = new Stopwatch();
                        sw.Start();

                        //if (useReversedMoves)
                        //    Move(array, i, x, 0, new Line(), MovesProvider._reversedMoves, ref lowest, ref visited);
                        //else
                            //Move(array, i, x, 0, new Line(), MovesProvider._moves, ref lowest, ref visited);
                        Move(array, i, x, 0, new LineModel(), moves, maxMoves, ref lowest, ref visited);

                        sw.Stop();
                        Console.WriteLine("Elapsed={0}", sw.Elapsed);
                        Console.WriteLine($"Optimal moves: {lowest}");
                        visited.DisplayLine();
                    }
                }
            }
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

        static void DisplayVisited(List<KeyValuePair<int,int>> visited)
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
