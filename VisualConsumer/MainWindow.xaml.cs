using FlowChartBuilder;
using FlowChartBuilder.Helpers;
using FlowChartBuilder.Models;
using Petzold.Media2D;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace VisualConsumer
{
    /// <summary>
    /// Logika interakcji dla klasy MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private static readonly int _rectangleWidth = 20;
        private static readonly int _rectangleHeight = 20;
        private static readonly int _lineThickness = 2;
        private static readonly int _multiplier = 65;
        private static readonly int _margin = 40;

        private List<INode> Nodes { get; set; }
        private Dictionary<int, int> OccupationOfRows { get; set; }
        private Dictionary<int, int> OccupationOfCols { get; set; }

        public MainWindow()
        {
            InitializeComponent();
            var filename = "and.gsa";
            List<INode> Nodes = null;
            List<LineModel> DirectLines = null;
            List<LineModel> IndirectLines = null;
            OccupationOfRows = new Dictionary<int, int>();
            OccupationOfCols = new Dictionary<int, int>();
            for (int i = 0; i < 6; i++)
            {
                OccupationOfRows.Add(i, 0);
                OccupationOfCols.Add(i, 0);
            }

            ChartDLLConnector.CreateGraphData(filename, ref Nodes, ref DirectLines, ref IndirectLines);
            this.Nodes = Nodes;

            foreach (var node in Nodes)
            {
                AddBlockWithName((int)node.GetPosition().y,
                    (int)node.GetPosition().x,
                    node.GetName(),
                    node.GetType());
            }

            var indirectLines = SortIndirectLines(IndirectLines);

            foreach (var line in DirectLines)
            {
                AddDirectLine(line);
            }

            foreach(var line in indirectLines)
            {
                AddIndirectLine(line);
            }


        }

        private void AddBlockWithName(int x, int y, string name, Type type)
        {
            Rectangle rect = new Rectangle();

            if (type == typeof(StartingNode))
                rect.Stroke = new SolidColorBrush(Colors.Green);
            else if (type == typeof(ProcessNode))
                rect.Stroke = new SolidColorBrush(Colors.Blue);
            else if (type == typeof(DecisionNode))
                rect.Stroke = new SolidColorBrush(Colors.Yellow);
            else if (type == typeof(EndingNode))
                rect.Stroke = new SolidColorBrush(Colors.Red);

            rect.Fill = new SolidColorBrush(Colors.White);
            rect.Width = _rectangleWidth;
            rect.Height = _rectangleHeight;
            rect.StrokeThickness = _lineThickness;

            TextBlock textBlock = new TextBlock();
            textBlock.Height = _rectangleHeight - _lineThickness;
            textBlock.Width = _rectangleWidth - _lineThickness;
            textBlock.Inlines.Add(new Run(name));
            textBlock.TextAlignment = TextAlignment.Center;

            myCanvas.Children.Add(rect);
            Canvas.SetTop(rect, y * _multiplier + _margin);
            Canvas.SetLeft(rect, x * _multiplier + _margin);

            myCanvas.Children.Add(textBlock);
            Canvas.SetTop(textBlock, y * _multiplier + _margin + _lineThickness);
            Canvas.SetLeft(textBlock, x * _multiplier + _margin + _lineThickness);
        }

        private void AddDirectLine(LineModel line)
        {
            ArrowPolyline apoly = new ArrowPolyline();
            RotateTransform xform = new RotateTransform();
            apoly.LayoutTransform = xform;
            apoly.ArrowEnds = ArrowEnds.Start;
            apoly.Stroke = Brushes.Black;
            apoly.StrokeThickness = _lineThickness;

            //foreach(var point in line.GetPointsOfLine())
            //{
            //    apoly.Points.Add(new Point(point.y * _multiplier + _margin + _rectangleWidth/2, point.x * _multiplier + _margin + _rectangleWidth / 2));
            //}

            var arrowPoint = line.GetPointsOfLine()[0];

            if (line.GetArrowDirection() == Direction.E)
                apoly.Points.Add(new Point(arrowPoint.y * _multiplier + _margin, arrowPoint.x * _multiplier + _margin + _rectangleWidth / 2));
            else if (line.GetArrowDirection() == Direction.N)
                apoly.Points.Add(new Point(arrowPoint.y * _multiplier + _margin + _rectangleWidth / 2, arrowPoint.x * _multiplier + _margin + _rectangleWidth));
            else if (line.GetArrowDirection() == Direction.S)
                apoly.Points.Add(new Point(arrowPoint.y * _multiplier + _margin + _rectangleWidth / 2, arrowPoint.x * _multiplier + _margin));
            else if (line.GetArrowDirection() == Direction.W)
                apoly.Points.Add(new Point(arrowPoint.y * _multiplier + _margin + _rectangleWidth, arrowPoint.x * _multiplier + _margin + _rectangleWidth / 2));

            for (int i = 1; i < line.GetPointsOfLine().Count; i++)
            {
                var point = line.GetPointsOfLine()[i];
                apoly.Points.Add(new Point(point.y * _multiplier + _margin + _rectangleWidth / 2, point.x * _multiplier + _margin + _rectangleWidth / 2));
            }

            myCanvas.Children.Add(apoly);
        }

        private void AddIndirectLine(LineModel line)
        {
            ArrowPolyline apoly = new ArrowPolyline();
            RotateTransform xform = new RotateTransform();
            apoly.LayoutTransform = xform;
            apoly.ArrowEnds = ArrowEnds.End;
            apoly.Stroke = Brushes.Black;
            apoly.StrokeThickness = _lineThickness;

            Direction? lastDirection = null;
            bool isSimple = true;
            var points = new List<Point>();
            points.Add(new Point(line.GetPointsOfLine().Last().y * _multiplier + _margin + _rectangleWidth/2, line.GetPointsOfLine().Last().x * _multiplier + _margin + _rectangleWidth/2));
            var prevPoint = line.GetPointsOfLine().Last();
            var skip = false;

            for (int i = line.GetPointsOfLine().Count - 2; i > 0; i--)
            {
                if (skip)
                    continue;


                var currPoint = line.GetPointsOfLine()[i];
                lastDirection = GetDirection(currPoint, prevPoint);
                var nextPoint = line.GetPointsOfLine()[i - 1];
                if (currPoint.Equals(prevPoint) /*&& line.GetPointsOfLine()[0].Equals(line.GetPointsOfLine()[1])*/ && line.GetPointsOfLine().Count >2 /*&& (line.GetPointsOfLine()[0].x == prevPoint.x || line.GetPointsOfLine()[0].y == prevPoint.y)*/)
                {
                    isSimple = false;
                    
                    var dir = GetDirection(nextPoint, currPoint);
                    lastDirection = dir;

                    if (dir.HasValue && (dir.Value == Direction.E || dir.Value == Direction.W))
                    {
                        var parent = GetNodeByCoordinates(prevPoint.x, prevPoint.y, this.Nodes);
                        if (parent.GetOccupiedSides() == null || (!parent.GetOccupiedSides().Contains(Direction.N) && !parent.GetOccupiedSides().Contains(Direction.S)))
                        {
                            if (this.OccupationOfRows[(int)currPoint.x] < this.OccupationOfRows[(int)currPoint.x - 1])
                            {
                                points.Add(new Point(currPoint.y * _multiplier + _margin + _rectangleWidth / 2, currPoint.x * _multiplier + _margin + _rectangleWidth / 2 + _multiplier / 2));
                                points.Add(new Point(nextPoint.y * _multiplier + _margin + _rectangleWidth / 2, nextPoint.x * _multiplier + _margin + _rectangleWidth / 2 + _multiplier / 2));
                                this.OccupationOfRows[(int)currPoint.x]++;
                            }
                            else
                            {
                                points.Add(new Point(currPoint.y * _multiplier + _margin + _rectangleWidth / 2, currPoint.x * _multiplier + _margin + _rectangleWidth / 2 - _multiplier / 2));
                                points.Add(new Point(nextPoint.y * _multiplier + _margin + _rectangleWidth / 2, nextPoint.x * _multiplier + _margin + _rectangleWidth / 2 - _multiplier / 2));
                                this.OccupationOfRows[(int)currPoint.x - 1]++;
                            }
                        }
                        else if (!parent.GetOccupiedSides().Contains(Direction.N))
                        {
                            points.Add(new Point(currPoint.y * _multiplier + _margin + _rectangleWidth / 2, currPoint.x * _multiplier + _margin + _rectangleWidth / 2 - _multiplier / 2));
                            points.Add(new Point(nextPoint.y * _multiplier + _margin + _rectangleWidth / 2, nextPoint.x * _multiplier + _margin + _rectangleWidth / 2 - _multiplier / 2));
                            this.OccupationOfRows[(int)currPoint.x - 1]++;
                        }
                        else if (!parent.GetOccupiedSides().Contains(Direction.S))
                        {
                            points.Add(new Point(currPoint.y * _multiplier + _margin + _rectangleWidth / 2, currPoint.x * _multiplier + _margin + _rectangleWidth / 2 + _multiplier / 2));
                            points.Add(new Point(nextPoint.y * _multiplier + _margin + _rectangleWidth / 2, nextPoint.x * _multiplier + _margin + _rectangleWidth / 2 + _multiplier / 2));
                            this.OccupationOfRows[(int)currPoint.x]++;
                        }
                    }
                    else if (dir.HasValue && (dir.Value == Direction.S || dir.Value == Direction.N))
                    {
                        var parent = GetNodeByCoordinates(prevPoint.x, prevPoint.y, this.Nodes);
                        if (parent.GetOccupiedSides() == null || (!parent.GetOccupiedSides().Contains(Direction.W) && !parent.GetOccupiedSides().Contains(Direction.E)))
                        {
                            if (this.OccupationOfCols[(int)currPoint.y] < this.OccupationOfCols[(int)currPoint.y - 1])
                            {
                                points.Add(new Point(currPoint.y * _multiplier + _margin + _rectangleWidth / 2 + _multiplier / 2, currPoint.x * _multiplier + _margin + _rectangleWidth / 2));
                                points.Add(new Point(nextPoint.y * _multiplier + _margin + _rectangleWidth / 2 + _multiplier / 2, nextPoint.x * _multiplier + _margin + _rectangleWidth / 2));
                                this.OccupationOfCols[(int)currPoint.x]++;
                            }
                            else
                            {
                                points.Add(new Point(currPoint.y * _multiplier + _margin + _rectangleWidth / 2 - _multiplier / 2, currPoint.x * _multiplier + _margin + _rectangleWidth / 2));
                                points.Add(new Point(nextPoint.y * _multiplier + _margin + _rectangleWidth / 2 - _multiplier / 2, nextPoint.x * _multiplier + _margin + _rectangleWidth / 2));
                                this.OccupationOfRows[(int)currPoint.x - 1]++;
                            }
                        }
                        else if (!parent.GetOccupiedSides().Contains(Direction.N))
                        {
                            points.Add(new Point(currPoint.y * _multiplier + _margin + _rectangleWidth / 2 - _multiplier / 2, currPoint.x * _multiplier + _margin + _rectangleWidth / 2));
                            points.Add(new Point(nextPoint.y * _multiplier + _margin + _rectangleWidth / 2 - _multiplier / 2, nextPoint.x * _multiplier + _margin + _rectangleWidth / 2));
                            this.OccupationOfRows[(int)currPoint.x - 1]++;
                        }
                        else if (!parent.GetOccupiedSides().Contains(Direction.S))
                        {
                            points.Add(new Point(currPoint.y * _multiplier + _margin + _rectangleWidth / 2 + _multiplier / 2, currPoint.x * _multiplier + _margin + _rectangleWidth / 2));
                            points.Add(new Point(nextPoint.y * _multiplier + _margin + _rectangleWidth / 2 + _multiplier / 2, nextPoint.x * _multiplier + _margin + _rectangleWidth / 2));
                            this.OccupationOfRows[(int)currPoint.x]++;
                        }
                    }

                    if (/*!nextPoint.Equals(GetNodeById(line.GetTargetNodeId(), this.Nodes).GetPosition())*/ i != 2 && i != 1)
                    {
                        lastDirection = GetDirection(nextPoint, currPoint);
                        //points.Remove(points.Last());
                        if (lastDirection == Direction.W)
                        {
                            var last = points.Last();
                            points.Remove(last);
                            points.Add(new Point(last.X - _multiplier / 2, last.Y));
                            //points.Add(last);
                            var contPoint = line.GetPointsOfLine()[i - 2];
                            points.Add(new Point(contPoint.y * _multiplier + _margin + _rectangleWidth / 2 - _multiplier / 2, contPoint.x * _multiplier + _margin + _rectangleWidth / 2));
                        }
                        skip = true;
                        isSimple = true;
                    }
                    prevPoint = currPoint;

                }


                else if (isSimple)
                {
                    if (lastDirection == null || GetNodeByCoordinates(currPoint.x, currPoint.y, this.Nodes) == null)
                    {
                        points.Add(new Point(currPoint.y * _multiplier + _margin + _rectangleWidth / 2, currPoint.x * _multiplier + _margin + _rectangleWidth / 2));
                        lastDirection = GetDirection(currPoint, prevPoint);
                        prevPoint = currPoint;
                    }
                    else
                    {
                        if (lastDirection == Direction.N)
                        {
                            points.Add(new Point(currPoint.y * _multiplier + _margin + _rectangleWidth / 2, currPoint.x * _multiplier + _margin + _rectangleWidth/2 + _multiplier/2));
                            points.Add(new Point(nextPoint.y * _multiplier + _margin + _rectangleWidth / 2, nextPoint.x * _multiplier + _margin + _rectangleWidth/ 2 + _multiplier / 2));
                            prevPoint = currPoint;
                            skip = true;
                            OccupationOfRows[(int)currPoint.x]++;
                        }
                        if (lastDirection == Direction.S)
                        {
                            points.Add(new Point(currPoint.y * _multiplier + _margin + _rectangleWidth / 2, currPoint.x * _multiplier + _margin + _rectangleWidth / 2 - _multiplier / 2));
                            points.Add(new Point(nextPoint.y * _multiplier + _margin + _rectangleWidth / 2, nextPoint.x * _multiplier + _margin + _rectangleWidth / 2 - _multiplier / 2));
                            prevPoint = currPoint;
                            skip = true;
                            OccupationOfRows[(int)currPoint.x]++;
                        }
                        if (lastDirection == Direction.W)
                        {
                            points.Add(new Point(currPoint.y * _multiplier + _margin + _rectangleWidth/ 2 + _multiplier / 2, currPoint.x * _multiplier + _margin + _rectangleWidth/2));
                            points.Add(new Point(nextPoint.y * _multiplier + _margin + _rectangleWidth/ 2 + _multiplier / 2, nextPoint.x * _multiplier + _margin + _rectangleWidth/2));
                            prevPoint = currPoint;
                            skip = true;
                            OccupationOfCols[(int)currPoint.y]++;
                        }
                        if (lastDirection == Direction.E)
                        {
                            points.Add(new Point(currPoint.y * _multiplier + _margin + _rectangleWidth / 2 - _multiplier / 2, currPoint.x * _multiplier + _margin + _rectangleWidth/2));
                            points.Add(new Point(nextPoint.y * _multiplier + _margin + _rectangleWidth / 2 - _multiplier / 2, nextPoint.x * _multiplier + _margin + _rectangleWidth/2));
                            prevPoint = currPoint;
                            skip = true;
                            OccupationOfCols[(int)currPoint.y]++;
                        }
                    }
                }

                if (i == 1 && !isSimple)
                {
                    if (line.GetPointsOfLine()[0].Equals(GetNodeById(line.GetTargetNodeId(), this.Nodes).GetPosition()) && !GetNodeById(line.GetTargetNodeId(), this.Nodes).HasIncomingArrow())
                    {
                        points.Add(new Point(line.GetPointsOfLine()[0].y * _multiplier + _margin + _rectangleWidth / 2, line.GetPointsOfLine()[0].x * _multiplier + _margin + _rectangleWidth / 2));
                        GetNodeById(line.GetTargetNodeId(), this.Nodes).SetIncomingArrow();
                        continue;
                    }
                    var targetNodePos = GetNodeById(line.GetTargetNodeId(), this.Nodes).GetPosition();
                    if (lastDirection == Direction.W && GetNodeById(line.GetTargetNodeId(), this.Nodes).GetIncomingSide() == Direction.E)
                    {
                        var last = points.Last();
                        points.Remove(last);
                        last.X -= _multiplier / 2;
                        points.Add(last);
                        points.Add(new Point(targetNodePos.y * _multiplier + _margin + _rectangleWidth / 2 + _multiplier / 2, targetNodePos.x * _multiplier + _margin + _rectangleWidth / 2));
                    }
                    if (lastDirection == Direction.W && GetNodeById(line.GetTargetNodeId(), this.Nodes).GetIncomingSide() == Direction.W)
                    {
                        var last = points.Last();
                        points.Remove(last);
                        last.X -= _multiplier / 2;
                        points.Add(last);
                        points.Add(new Point(targetNodePos.y * _multiplier + _margin + _rectangleWidth / 2 - _multiplier / 2, targetNodePos.x * _multiplier + _margin + _rectangleWidth / 2));
                    }

                }
            }

            if (isSimple && !GetNodeById(line.GetTargetNodeId(), this.Nodes).HasIncomingArrow() && line.GetPointsOfLine()[0].Equals(GetNodeById(line.GetTargetNodeId(), this.Nodes).GetPosition()))
            {
                points.Add(new Point(GetNodeById(line.GetTargetNodeId(), this.Nodes).GetPosition().y * _multiplier + _margin + _rectangleWidth / 2, GetNodeById(line.GetTargetNodeId(), this.Nodes).GetPosition().x * _multiplier + _margin + _rectangleWidth / 2));
            }

            foreach (var point in points)
            {
                apoly.Points.Add(point);
            }
            myCanvas.Children.Add(apoly);
        }

        private static Direction? GetDirection(Coordinates a, Coordinates b)
        {
            if (a.x == b.x && a.y < b.y)
            {
                return Direction.W;
            }
            if (a.x == b.x && a.y > b.y)
            {
                return Direction.E;
            }
            if (a.x < b.x && a.y == b.y)
            {
                return Direction.N;
            }
            if (a.x > b.x && a.y == b.y)
            {
                return Direction.S;
            }
            return null;
        }

        private INode GetNodeByCoordinates(double x, double y, List<INode> nodes)
        {
            return nodes.Where(n => n.GetPosition().x == x && n.GetPosition().y == y).FirstOrDefault();
        }

        private INode GetNodeById(int id, List<INode> nodes)
        {
            return nodes.Where(n => n.GetId() == id).FirstOrDefault();
        }

        private List<LineModel> SortIndirectLines(List<LineModel> lines)
        {
            var output = new List<LineModel>();
            var self = new List<LineModel>();
            var backComing = new List<LineModel>();
            var nonBackComing = new List<LineModel>();

            foreach (var line in lines)
            {
                var points = line.GetPointsOfLine();
                if (points.Count == 3 && points[0].Equals(points[1]) && points[0].Equals(points[2]))
                {
                    self.Add(line);
                }
                else if (points.Count >= 4 && points[0].Equals(points[1]) && points[points.Count - 1] == points[points.Count - 2] && (points[0].x == points.Last().x || points[0].y == points.Last().y)){
                    backComing.Add(line);
                }
                else
                    nonBackComing.Add(line);
            }

            output.AddRange(self);
            output.AddRange(nonBackComing);
            output.AddRange(backComing);
            return output;
        }
    }
}
