using FlowChartBuilder.Models;
using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Helpers
{
    class GraphMaker
    {
        private List<Line> Lines { get; set; }
        private int[,] Grid { get; set; }
        private List<INode> Nodes { get; set; }
        private List<Connection> Connections { get; set; }

        public GraphMaker(List<INode> nodes, int[,] grid)
        {
            this.Nodes = nodes;
            this.Grid = grid;
            this.Lines = new List<Line>();
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
            }


            //var gridCopy = Grid.Clone() as int[,];

            foreach (var connection in Connections)
            {
                var gridCopy = Grid.Clone() as int[,];
                int x = 0, y = 0;

                for (int i = 0; i < gridCopy.GetLength(0); i++)
                {
                    for (int j = 0; j < gridCopy.GetLength(1); j++)
                    {
                        if (gridCopy[i, j] == connection.GetFirstNodeId())
                        {
                            gridCopy[i, j] = 1;
                            x = i;
                        }
                        else if (gridCopy[i, j] == connection.GetSecondNodeId())
                        {
                            gridCopy[i, j] = 2;
                            y = i;
                        } 
                        else if (gridCopy[i, j] > 0)
                        {
                            gridCopy[i, j] = int.MaxValue;
                        }
                    }
                }

                Line newLine = null;
                if (x > y)
                {
                    newLine = LeeAlgorithmInterpreter.DoYourJob(gridCopy, true);
                }
                else
                {
                    newLine = LeeAlgorithmInterpreter.DoYourJob(gridCopy, false);
                }
                Lines.Add(newLine);

                //gridCopy = Grid.Clone() as int[,];
                //foreach (var line in Lines)
                //{
                //    for (int i = 0; i < line.GetPointsOfLine().Count - 1; i++)
                //    {
                //        gridCopy[line.GetPointsOfLine()[i].x, line.GetPointsOfLine()[i].y] = int.MaxValue;
                //    }
                //}

            }
        }
    }
}
