using FlowChartBuilder;
using FlowChartBuilder.Helpers;
using FlowChartBuilder.Models;
using Petzold.Media2D;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
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

namespace UI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private static readonly int _rectangleWidth = 20;
        private static readonly int _rectangleHeight = 20;
        private static readonly int _lineThickness = 2;
        private static readonly int _multiplier = 20;

        private List<LineModel> Lines { get; set; }
        private List<INode> Nodes { get; set; } 
        private List<VectorModel> Vectors { get; set; }
        private Queue<VectorModel> Recheck { get; set; }
        private Brush[] BrushesArray { get; set; }

        public MainWindow()
        {
            InitializeComponent();

            this.Vectors = new List<VectorModel>();

            BrushesArray = new Brush[80];
            for (int i = 0; i < 80; i++)
            {
                BrushesArray[i] = PickBrush();
            }

            //var path = AppDomain.CurrentDomain.BaseDirectory + @"testfiles\test3.txt";
            var path = (AppDomain.CurrentDomain.BaseDirectory + @"Examples\and.gsa").Replace(@"UI\bin\Debug\netcoreapp3.1\", "");
            var list = TextFileParser.ParseText(path);//.Replace(@"UI\bin\Debug\netcoreapp3.1\", ""));
            var grid = new BlockDistributor(list);
            grid.RemoveEmptyLines();
            grid.PrintGrid();
            grid.SetNodesPositions();

            var graph = new GraphMaker(grid.GetNodes(), grid.GetGrid());
            Stopwatch sw = new Stopwatch();
            sw.Start();
            graph.LineCreator();
            sw.Stop();
            Console.WriteLine("ElapsedTotal={0}", sw.Elapsed);

            graph.RemoveMiddlePointsFromLines();

            this.Lines = graph.GetLines();
            this.Nodes = graph.GetNodes();

            foreach (var line in Lines)
            {
                ArrowPolyline apoly = new ArrowPolyline();
                RotateTransform xform = new RotateTransform();
                apoly.LayoutTransform = xform;
                if (line.IsReversed)
                    apoly.ArrowEnds = ArrowEnds.Start;
                else
                    apoly.ArrowEnds = ArrowEnds.End;
                apoly.Stroke = Brushes.Green;
                apoly.StrokeThickness = 3;


                foreach (var point in line.GetPointsOfLine())
                    apoly.Points.Add(new Point((point.y * _multiplier) +_rectangleWidth / 2 + 20, (point.x * _multiplier) + _rectangleWidth / 2 + 20));
                myCanvas.Children.Add(apoly);
            }

            //int id = 1;
            //foreach (var line in Lines)
            //{
            //    var points = line.GetPointsOfLine();
            //    for (int i = 0; i < points.Count - 1; i++)
            //    {
            //        Vectors.Add(new VectorModel(new Coordinates(points[i].x * _multiplier, points[i].y * _multiplier), new Coordinates(points[i + 1].x * _multiplier, points[i + 1].y * _multiplier), id));
            //    }
            //    id++;
            //}

            //this.Vectors = this.Vectors.Where(x => x.Id <= 100).ToList();
            //this.Recheck = new Queue<VectorModel>();
            //foreach (var vector in Vectors)
            //{
            //    AddLine(vector);
            //}

            //while (this.Recheck.Count != 0)
            //{
            //    var recheck = this.Recheck.Dequeue();
            //    if (recheck.GetChangeNumber() < 5)
            //    {
            //        recheck.AddChange();
            //        AddLine(recheck);
            //    }
            //}

            //foreach (var vector in Vectors)
            //{
            //    DrawVector(vector);
            //}

            foreach (var node in Nodes)
            {
                //AddBlock(node.GetPosition().y, node.GetPosition().x, node.GetId(), node.GetType());
                AddBlockWithName(node.GetPosition().y, node.GetPosition().x, node.GetName(), node.GetType());
            }
        }


        private void AddLine(VectorModel vector)
        {
            var mover = (int)_multiplier / 4 - 1;

            var x = Vectors.Where(x => ((x.Start.y < vector.Start.y && x.End.y > vector.Start.y) || (x.Start.y < vector.End.y && x.End.y > vector.End.y) || (x.Start.y == vector.Start.y && x.End.y == vector.End.y)) && x.Start.x == vector.Start.x && x.Id != vector.Id && !IsVertical(x) && !IsVertical(vector)).ToList();
            var y = Vectors.Where(x => ((x.Start.x < vector.Start.x && x.End.x > vector.Start.x) || (x.Start.x < vector.End.x && x.End.x > vector.End.x) || (x.Start.x == vector.Start.x && x.End.x == vector.End.x)) && x.Start.y == vector.Start.y && x.Id != vector.Id && IsVertical(x) && IsVertical(vector)).ToList();

            if (x.Count >= 1 && !IsVertical(vector))
            {

                vector.AddChange();
                if (x.Count % 2 != 0 || vector.GetChangeNumber() % 2 != 0)
                    MoveVectorsWithId(vector.Id, -mover * (int)(x.Count + 1 / 2 + vector.GetChangeNumber() / 2), 0);
                else
                    MoveVectorsWithId(vector.Id, mover * (int)(x.Count / 2 + vector.GetChangeNumber() / 2), 0);
                if (!this.Recheck.Where(x => x.Id == vector.Id).Any())
                {
                    foreach (var recheck in Vectors.Where(x => x.Id == vector.Id))
                    {
                        //if (!this.Recheck.Contains(recheck))
                        this.Recheck.Enqueue(recheck);
                    }
                }
            }

            if (y.Count >= 1 && IsVertical(vector))
            {
                vector.AddChange();
                if (y.Count % 2 != 0 || vector.GetChangeNumber() % 2 != 0)
                    MoveVectorsWithId(vector.Id, 0, -mover * (int)(y.Count + 1) / 2);
                else
                    MoveVectorsWithId(vector.Id, 0, mover * (int)y.Count / 2);

                if (!this.Recheck.Where(x => x.Id == vector.Id).Any())
                {
                    foreach (var recheck in Vectors.Where(x => x.Id == vector.Id))
                    {
                        //if (!this.Recheck.Contains(recheck))
                        this.Recheck.Enqueue(recheck);
                    }
                }
            }
        }

        private void DrawVector(VectorModel vector)
        {
            Line line = new System.Windows.Shapes.Line();
            line.StrokeThickness = _lineThickness;
            //line.Stroke = BrushesArray[vector.Id];
            line.Stroke = Brushes.Black;
            line.X1 = vector.Start.y + _rectangleWidth / 2 + 20;
            line.X2 = vector.End.y + _rectangleWidth / 2 + 20;
            line.Y1 = vector.Start.x + _rectangleHeight / 2 + 20;
            line.Y2 = vector.End.x + _rectangleHeight / 2 + 20;

            myCanvas.Children.Add(line);
        }

        private void MoveVectorsWithId(int id, int moveX, int moveY)
        {
            var vects = this.Vectors.Where(x => x.Id == id);
            foreach (var v in vects)
            {
                v.Start.x += moveX;
                v.End.x += moveX;
                v.Start.y += moveY;
                v.End.y += moveY;
            }
        }

        private bool IsVertical(VectorModel vector)
        {
            if (vector.Start.x == vector.End.x)
                return false;
            return true;
        }

        private void AddBlock(int x, int y, int id, Type type)
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
            textBlock.Inlines.Add(new Run(id.ToString()));
            textBlock.TextAlignment = TextAlignment.Center;

            myCanvas.Children.Add(rect);
            Canvas.SetTop(rect, y * _multiplier + 20);
            Canvas.SetLeft(rect, x * _multiplier + 20);

            myCanvas.Children.Add(textBlock);
            Canvas.SetTop(textBlock, y * _multiplier + 20 + _lineThickness);
            Canvas.SetLeft(textBlock, x * _multiplier + 20 + _lineThickness);
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
            Canvas.SetTop(rect, y * _multiplier + 20);
            Canvas.SetLeft(rect, x * _multiplier + 20);

            myCanvas.Children.Add(textBlock);
            Canvas.SetTop(textBlock, y * _multiplier + 20 + _lineThickness);
            Canvas.SetLeft(textBlock, x * _multiplier + 20 + _lineThickness);
        }

        private Brush PickBrush()
        {
            Brush result = Brushes.Transparent;

            Random rnd = new Random();

            Type brushesType = typeof(Brushes);

            PropertyInfo[] properties = brushesType.GetProperties();

            int random = rnd.Next(properties.Length);
            result = (Brush)properties[random].GetValue(null, null);

            return result;
        }
    }
}
