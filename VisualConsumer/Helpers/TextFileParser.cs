using FlowChartBuilder.Models;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace FlowChartBuilder.Helpers
{
    public class TextFileParser
    {
        public static List<INode> ParseTextToNodes(string filePath)
        {
            List<INode> createdNodes = new List<INode>();
            string line;

            if (filePath.Contains(".gsa"))
            {
                filePath = filePath.Split('.')[0];
            }
            System.IO.StreamReader file = new System.IO.StreamReader(filePath + "-nodes.txt");
            
            while ((line = file.ReadLine()) != null)
            {
                System.Console.WriteLine(line);
                createdNodes.Add(GetNodeFromFile(line.Split((char[])null, StringSplitOptions.RemoveEmptyEntries)));
            }
            file.Close();

            return createdNodes;
        }

        public static void ParseTextToLines(string filePath, ref List<LineModel> directLines, ref List<LineModel> indirectLines)
        {
            indirectLines = new List<LineModel>();
            directLines = new List<LineModel>();
            string line;

            if (filePath.Contains(".gsa"))
            {
                filePath = filePath.Split('.')[0];
            }
            System.IO.StreamReader file = new System.IO.StreamReader(filePath + "-lines.txt");

            while ((line = file.ReadLine()) != null)
            {
                List<LineModel> indirects = null;
                System.Console.WriteLine(line);
                var direct = GetLineFromFile(line.Split(new string[] { "||" }, StringSplitOptions.None), ref indirects);
                if (direct != null)
                {
                    directLines.Add(direct);
                }
                indirectLines.AddRange(indirects);
            }
            file.Close();
        }

        private static INode GetNodeFromFile(string[] nodeData)
        {
            var nodeType = nodeData[1].ToLower();
            if (nodeType == "begin")
            {
                var startingNode = new StartingNode(int.Parse(nodeData[0]));
                startingNode.SetName(nodeData[1]);
                startingNode.SetPosition(int.Parse(nodeData[2]), int.Parse(nodeData[3]));
                return startingNode;
            }
            else if (nodeType == "end")
            {
                var endingNode = new EndingNode(int.Parse(nodeData[0]));
                endingNode.SetName(nodeData[1]);
                endingNode.SetPosition(int.Parse(nodeData[2]), int.Parse(nodeData[3]));
                return endingNode;
            }
            else if (new Regex(@"y[0-9]+").IsMatch(nodeType))
            {
                var processNode = new ProcessNode(int.Parse(nodeData[0]));
                processNode.SetPosition(int.Parse(nodeData[2]), int.Parse(nodeData[3]));
                processNode.SetName(nodeData[1]);
                return processNode;
            }
            else if (new Regex(@"x[0-9]+").IsMatch(nodeType))
            {
                var decisionNode = new DecisionNode(int.Parse(nodeData[0]));
                decisionNode.SetPosition(int.Parse(nodeData[2]), int.Parse(nodeData[3]));
                decisionNode.SetName(nodeData[1]);
                return decisionNode;
            }
            else return null;
        }

        private static LineModel GetLineFromFile(string[] lineData, ref List<LineModel> indirectLineModels)
        {
            LineModel directLine = null;
            indirectLineModels = new List<LineModel>();

            var targetNodeData = lineData[0].Split((char[])null, StringSplitOptions.RemoveEmptyEntries);
            var directLinePoints = lineData[1].Split((char[])null, StringSplitOptions.RemoveEmptyEntries);
            var indirectLines = lineData[2].Split('|');

            if (directLinePoints.Length != 0)
            {
                directLine = new LineModel();

                foreach (var point in directLinePoints)
                {
                    var coords = point.Split(',');
                    directLine.AddPointToLine(int.Parse(coords[0]), int.Parse(coords[1]));
                }
                directLine.SetArrowDirection(GetDirection(directLine.GetPointsOfLine()[1], directLine.GetPointsOfLine()[0]));
                directLine.SetTargetNodeId(int.Parse(targetNodeData[0]));
            }

            foreach (var line in indirectLines)
            {
                if (String.IsNullOrEmpty(line))
                    continue;
                var indirectLine = new LineModel();
                foreach (var point in line.Split((char[])null, StringSplitOptions.RemoveEmptyEntries))
                {
                    var coords = point.Split(',');
                    indirectLine.AddPointToLine(int.Parse(coords[0]), int.Parse(coords[1]));
                }
                indirectLine.SetTargetNodeId(int.Parse(targetNodeData[0]));
                indirectLineModels.Add(indirectLine);
            }

            return directLine;
        }

        private static Direction? GetDirection(Coordinates a, Coordinates b)
        {
            if (a.x == b.x && a.y < b.y)
            {
                return Direction.E;
            }
            if (a.x == b.x && a.y > b.y)
            {
                return Direction.W;
            }
            if (a.x < b.x && a.y == b.y)
            {
                return Direction.S;
            }
            if (a.x > b.x && a.y == b.y)
            {
                return Direction.N;
            }
            return null;
        }

        public static void SetOccupiedSides(List<INode> nodes, List<LineModel> lines)
        {
            foreach (var line in lines)
            {
                if (line.GetArrowDirection() != null)
                {
                    if (line.GetArrowDirection().Value == Direction.W)
                    {
                        GetNodeById(line.GetTargetNodeId(), nodes).AddOccupiedSide(Direction.E);
                        GetNodeById(line.GetTargetNodeId(), nodes).SetIncomingSide(Direction.E);
                        GetNodeByCoordinates(line.GetPointsOfLine().Last(), nodes).AddOccupiedSide(Direction.W);
                    }
                    else if (line.GetArrowDirection().Value == Direction.E)
                    {
                        GetNodeById(line.GetTargetNodeId(), nodes).AddOccupiedSide(Direction.W);
                        GetNodeById(line.GetTargetNodeId(), nodes).SetIncomingSide(Direction.W);
                        GetNodeByCoordinates(line.GetPointsOfLine().Last(), nodes).AddOccupiedSide(Direction.E);
                    }
                    if (line.GetArrowDirection().Value == Direction.S)
                    {
                        GetNodeById(line.GetTargetNodeId(), nodes).AddOccupiedSide(Direction.N);
                        GetNodeById(line.GetTargetNodeId(), nodes).SetIncomingSide(Direction.N);
                        GetNodeByCoordinates(line.GetPointsOfLine().Last(), nodes).AddOccupiedSide(Direction.S);
                    }
                    if (line.GetArrowDirection().Value == Direction.N)
                    {
                        GetNodeById(line.GetTargetNodeId(), nodes).AddOccupiedSide(Direction.S);
                        GetNodeById(line.GetTargetNodeId(), nodes).SetIncomingSide(Direction.S);
                        GetNodeByCoordinates(line.GetPointsOfLine().Last(), nodes).AddOccupiedSide(Direction.N);
                    }
                }
                GetNodeById(line.GetTargetNodeId(), nodes).SetIncomingArrow();
            }
        }

        private static INode GetNodeByCoordinates(Coordinates coords, List<INode> nodes)
        {
            return nodes.Where(n => n.GetPosition().x == coords.x && n.GetPosition().y == coords.y).FirstOrDefault();
        }

        private static INode GetNodeById(int id, List<INode> nodes)
        {
            return nodes.Where(n => n.GetId() == id).FirstOrDefault();
        }
    }
}
