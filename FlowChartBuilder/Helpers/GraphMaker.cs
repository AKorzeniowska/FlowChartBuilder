using FlowChartBuilder.Models;
using FlowChartBuilder.Providers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FlowChartBuilder.Helpers
{
    public class GraphMaker
    {
        private List<LineModel> Lines { get; set; }
        private int[,] Grid { get; set; }
        private List<INode> Nodes { get; set; }
        private List<Connection> Connections { get; set; }

        public List<LineModel> GetLines()
        {
            return this.Lines;
        }

        public List<INode> GetNodes()
        {
            this.Nodes.RemoveAll(x => x.GetType() == typeof(SubsidiaryNode));
            return this.Nodes;
        }


        public GraphMaker(List<INode> nodes, int[,] grid)
        {
            this.Nodes = nodes;
            this.Grid = grid;
            this.Lines = new List<LineModel>();
            this.Connections = new List<Connection>();
        }

        public void LineCreator()
        {
            foreach(var node in Nodes)
            {
                if (node.GetType() == typeof(StartingNode))
                {
                    Connections.Add(new Connection(node.GetId(), (node as StartingNode).GetFollowingNodeId()));
                }
                else if (node.GetType() == typeof(ProcessNode))
                {
                    Connections.Add(new Connection(node.GetId(), (node as ProcessNode).GetFollowingNodeId()));
                }
                else if (node.GetType() == typeof(DecisionNode))
                {
                    Connections.Add(new Connection(node.GetId(), (node as DecisionNode).GetLeftFollowingNodeId()));
                    Connections.Add(new Connection(node.GetId(), (node as DecisionNode).GetRightFollowingNodeId()));
                }
                else if (node.GetType() == typeof(SubsidiaryNode))
                {
                    Connections.Add(new Connection(node.GetId(), (node as SubsidiaryNode).GetFollowingNodeId()));
                }
            }

            var redo = SetLines(Connections);
            var redo2 = SetLines(redo);
            var redoLast = SetLines(redo2);
            SetLinesWithoutObstacles(redoLast);
        }

        private Dictionary<int, Connection> SetLines(List<Connection> connections)
        {
            var gridCopy = Grid.Clone() as int[,];
            int id = 1;

            var redo = new Dictionary<int, Connection>();
            bool cont = true;
            connections.Reverse();
            var shuffled = connections.OrderBy(x => new Random().Next()).ToList();
            foreach (var connection in /*shuffled*/connections)
            {
                //var gridCopy = Grid.Clone() as int[,];
                int x1 = 0, x2 = 0, y1 = 0, y2 = 0;

                for (int i = 0; i < gridCopy.GetLength(0); i++)
                {
                    for (int j = 0; j < gridCopy.GetLength(1); j++)
                    {
                        if (gridCopy[i, j] == connection.GetFirstNodeId())
                        {
                            if (j > 0 && i > 0 && j < gridCopy.GetLength(1) - 1 && i < gridCopy.GetLength(0) - 1 && gridCopy[i, j - 1] == int.MaxValue && gridCopy[i, j + 1] == int.MaxValue && gridCopy[i - 1, j] == int.MaxValue && gridCopy[i + 1, j] == int.MaxValue)
                            {
                                if (!redo.ContainsKey(id))
                                    redo.Add(id, connection);
                                cont = false;
                            }
                            gridCopy[i, j] = 1;
                            x1 = i;
                            y1 = j;
                        }
                        else if (gridCopy[i, j] == connection.GetSecondNodeId())
                        {
                            if (j > 0 && i > 0 && j < gridCopy.GetLength(1) - 1 && i < gridCopy.GetLength(0) - 1 && gridCopy[i, j - 1] == int.MaxValue && gridCopy[i, j + 1] == int.MaxValue && gridCopy[i - 1, j] == int.MaxValue && gridCopy[i + 1, j] == int.MaxValue)
                            {
                                if (!redo.ContainsKey(id))
                                    redo.Add(id, connection);
                                cont = false;
                            }
                            gridCopy[i, j] = 2;
                            x2 = i;
                            y2 = j;
                        }
                        else if (gridCopy[i, j] > 0)
                        {
                            gridCopy[i, j] = int.MaxValue;
                        }
                    }
                }

                if ((x2 == 0 && y2 == 0) || (x1 == 0 && y1 == 0)) {
                    id++;
                    gridCopy = Grid.Clone() as int[,];
                    continue; 
                }

                if (cont)
                {
                    LineModel newLine = null;
                    int[][] moves1 = null;
                    int[][] moves2 = null;

                    if (x1 > x2 && y1 >= y2)
                    {
                        moves1 = MovesProvider.UpLeft;
                        moves2 = MovesProvider.LeftUp;
                    }
                    else if (x1 > x2 && y1 < y2)
                    {
                        moves1 = MovesProvider.UpRight;
                        moves2 = MovesProvider.RightUp;
                    }
                    else if (x1 <= x2 && y1 >= y2)
                    {
                        moves1 = MovesProvider.DownLeft;
                        moves2 = MovesProvider.LeftDown;
                    }
                    else if (x1 <= x2 && y1 < y2)
                    {
                        moves1 = MovesProvider.DownRight;
                        moves2 = MovesProvider.RightDown;
                    }

                    var optTurns = Math.Abs(x1 - x2) == 0 || Math.Abs(y1 - y2) == 0 ? 0 : 1;

                    newLine = new LeeAlgorithmInterpreter().DoYourJob(gridCopy, moves1, moves2, /*(Math.Abs(x1 - x2) + Math.Abs(y1 - y2)) + 4*/
                        Grid.GetLength(1) + Grid.GetLength(0), optTurns, id, x1, y1, x2, y2);
                    if (newLine.GetPointsOfLine().Count != 0)
                    {
                        Lines.Add(newLine);
                        if (!newLine.IsReversed)
                            newLine.AddPointAtStartOfLine(x1, y1);
                        else
                            newLine.AddPointAtEndOfLine(x2, y2);
                    }
                    else
                        redo.Add(id, connection);
                }
                else
                {
                    cont = true;
                }
                id++;
                gridCopy = Grid.Clone() as int[,];
                foreach (var line in Lines)
                {
                    for (int i = 1; i < line.GetPointsOfLine().Count - 1; i++)
                    {
                        gridCopy[line.GetPointsOfLine()[i].x, line.GetPointsOfLine()[i].y] = int.MaxValue;
                    }
                }
            }
            return redo;
        }

        private Dictionary<int, Connection> SetLines(Dictionary<int, Connection> connections)
        {
            var gridCopy = Grid.Clone() as int[,];
            var thirdIterationLines = new List<LineModel>();
            var redoLast = new Dictionary<int, Connection>();
            foreach (var connectionWithId in connections)
            {
                int x1 = 0, x2 = 0, y1 = 0, y2 = 0;

                for (int i = 0; i < gridCopy.GetLength(0); i++)
                {
                    for (int j = 0; j < gridCopy.GetLength(1); j++)
                    {
                        if (gridCopy[i, j] == connectionWithId.Value.GetFirstNodeId())
                        {
                            gridCopy[i, j] = 1;
                            x1 = i;
                            y1 = j;
                        }
                        else if (gridCopy[i, j] == connectionWithId.Value.GetSecondNodeId())
                        {
                            gridCopy[i, j] = 2;
                            x2 = i;
                            y2 = j;
                        }
                        else if (gridCopy[i, j] > 0)
                        {
                            gridCopy[i, j] = int.MaxValue;
                        }
                    }
                }

                LineModel newLine = null;
                int[][] moves1 = null;
                int[][] moves2 = null;

                if (x1 > x2 && y1 >= y2)
                {
                    moves1 = MovesProvider.UpLeft;
                    moves2 = MovesProvider.LeftUp;
                }
                else if (x1 > x2 && y1 < y2)
                {
                    moves1 = MovesProvider.UpRight;
                    moves2 = MovesProvider.RightUp;
                }
                else if (x1 <= x2 && y1 >= y2)
                {
                    moves1 = MovesProvider.DownLeft;
                    moves2 = MovesProvider.LeftDown;
                }
                else if (x1 <= x2 && y1 < y2)
                {
                    moves1 = MovesProvider.DownRight;
                    moves2 = MovesProvider.RightDown;
                }

                var optTurns = Math.Abs(x1 - x2) == 0 || Math.Abs(y1 - y2) == 0 ? 0 : 1;

                newLine = new LeeAlgorithmInterpreter().DoYourJob(gridCopy, moves1, moves2, gridCopy.GetLength(0) + gridCopy.GetLength(1), optTurns, connectionWithId.Key, x1, y1, x2, y2);
                if (newLine.GetPointsOfLine().Count != 0)
                {
                    Lines.Add(newLine);
                    if (!newLine.IsReversed)
                        newLine.AddPointAtStartOfLine(x1, y1);
                    else
                        newLine.AddPointAtEndOfLine(x2, y2);
                }
                else
                    redoLast.Add(connectionWithId.Key, connectionWithId.Value);

                thirdIterationLines.Add(newLine);
                gridCopy = Grid.Clone() as int[,];
                foreach (var line in thirdIterationLines)
                {
                    for (int i = 1; i < line.GetPointsOfLine().Count - 1; i++)
                    {
                        gridCopy[line.GetPointsOfLine()[i].x, line.GetPointsOfLine()[i].y] = int.MaxValue;
                    }
                }
            }
            return redoLast;
        }

        private void SetLinesWithoutObstacles(Dictionary<int, Connection> connections)
        {
            var gridCopy = Grid.Clone() as int[,];
            foreach (var connectionWithId in connections)
            {
                int x1 = 0, x2 = 0, y1 = 0, y2 = 0;

                for (int i = 0; i < gridCopy.GetLength(0); i++)
                {
                    for (int j = 0; j < gridCopy.GetLength(1); j++)
                    {
                        if (gridCopy[i, j] == connectionWithId.Value.GetFirstNodeId())
                        {
                            gridCopy[i, j] = 1;
                            x1 = i;
                            y1 = j;
                        }
                        else if (gridCopy[i, j] == connectionWithId.Value.GetSecondNodeId())
                        {
                            gridCopy[i, j] = 2;
                            x2 = i;
                            y2 = j;
                        }
                        else if (gridCopy[i, j] > 0)
                        {
                            gridCopy[i, j] = int.MaxValue;
                        }
                    }
                }

                LineModel newLine = null;
                int[][] moves1 = null;
                int[][] moves2 = null;

                if (x1 > x2 && y1 >= y2)
                {
                    moves1 = MovesProvider.UpLeft;
                    moves2 = MovesProvider.LeftUp;
                }
                else if (x1 > x2 && y1 < y2)
                {
                    moves1 = MovesProvider.UpRight;
                    moves2 = MovesProvider.RightUp;
                }
                else if (x1 <= x2 && y1 >= y2)
                {
                    moves1 = MovesProvider.DownLeft;
                    moves2 = MovesProvider.LeftDown;
                }
                else if (x1 <= x2 && y1 < y2)
                {
                    moves1 = MovesProvider.DownRight;
                    moves2 = MovesProvider.RightDown;
                }

                var optTurns = Math.Abs(x1 - x2) == 0 || Math.Abs(y1 - y2) == 0 ? 0 : 1;

                newLine = new LeeAlgorithmInterpreter().DoYourJob(gridCopy, moves1, moves2, (Math.Abs(x1 - x2) + Math.Abs(y1 - y2)) + 4, optTurns, connectionWithId.Key, x1, y1, x2, y2);
                if (newLine.GetPointsOfLine().Count != 0)
                {
                    Lines.Add(newLine);
                    if (!newLine.IsReversed)
                        newLine.AddPointAtStartOfLine(x1, y1);
                    else
                        newLine.AddPointAtEndOfLine(x2, y2);
                }
            }
        }

        public void RemoveMiddlePointsFromLines()
        {
            foreach (var line in this.Lines)
            {
                line.RemoveMiddlePoints();
            }
        }
    }
}
