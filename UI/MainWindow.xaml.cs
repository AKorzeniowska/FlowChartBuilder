using FlowChartBuilder;
using FlowChartBuilder.Helpers;
using FlowChartBuilder.Models;
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

namespace UI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private static readonly int _rectangleWidth = 20;
        private static readonly int _rectangleHeight = 20;
        private static readonly int _lineThickness = 3;
        private static readonly int _multiplier = 25;

        private List<LineModel> Lines { get; set; }
        private List<INode> Nodes { get; set; } 
        private List<VectorModel> Vectors { get; set; }
        public MainWindow()
        {
            InitializeComponent();

            this.Vectors = new List<VectorModel>();

            var path = AppDomain.CurrentDomain.BaseDirectory + @"testfiles\test3.txt";
            var list = TextFileParser.ParseText(path.Replace(@"UI\bin\Debug\netcoreapp3.1\", ""));
            var grid = new BlockDistributor(list);
            grid.PrintGrid();
            grid.RemoveEmptyLines();
            grid.PrintGrid();
            grid.SetNodesPositions();

            var graph = new GraphMaker(grid.GetNodes(), grid.GetGrid());
            graph.LineCreator();
            graph.RemoveMiddlePointsFromLines();

            this.Lines = graph.GetLines();
            this.Nodes = graph.GetNodes();

            foreach (var line in Lines)
            {
                var points = line.GetPointsOfLine();
                for (int i = 0; i < points.Count - 1; i++)
                {
                    Vectors.Add(new VectorModel(new Coordinates(points[i].x * _multiplier, points[i].y * _multiplier), new Coordinates(points[i + 1].x * _multiplier, points[i + 1].y * _multiplier)));
                }
            }

            foreach (var node in Nodes)
            {
                AddBlock(node.GetPosition().y, node.GetPosition().x);
            }

            foreach (var vector in Vectors)
            {
                AddLine(vector);
            }
        }
        private void AddLine(VectorModel vector)
        {
            int modifierx = 0;
            int modifiery = 0;

            var x = Vectors.Where(x => ((x.Start.y < vector.Start.y && x.End.y > vector.Start.y) || (x.Start.y < vector.End.y && x.End.y > vector.End.y)) && IsVertical(x)).ToList();
            var y = Vectors.Where(x => ((x.Start.x < vector.Start.x && x.End.x > vector.Start.x) || (x.Start.x < vector.End.x && x.End.x > vector.End.x)) && !IsVertical(x)).ToList();

            if (IsVertical(vector) && Vectors.Where(x => ((x.Start.y < vector.Start.y && x.End.y > vector.Start.y) || (x.Start.y < vector.End.y && x.End.y > vector.End.y)) && IsVertical(x)).Count() >= 2)
            {
                modifierx = 20;
                vector.End.x += 20;
                vector.Start.x += 20;
            }

            if (!IsVertical(vector) && Vectors.Where(x => ((x.Start.x < vector.Start.x && x.End.x > vector.Start.x) || (x.Start.x < vector.End.x && x.End.x > vector.End.x)) && !IsVertical(x)).Count() >= 2)
            {
                modifiery = 20;
                vector.End.y += 20;
                vector.Start.y += 20;
            }

            Line line = new System.Windows.Shapes.Line();
            line.StrokeThickness = _lineThickness;
            line.Stroke = System.Windows.Media.Brushes.Black;
            line.X1 = vector.Start.y * _multiplier + _rectangleWidth / 2 + 20 + modifierx;
            line.X2 = vector.End.y * _multiplier + _rectangleWidth / 2 + 20 + modifierx;
            line.Y1 = vector.Start.x * _multiplier + _rectangleHeight / 2 + 20 + modifiery;
            line.Y2 = vector.End.x * _multiplier + _rectangleHeight / 2 + 20 + modifiery;

            myCanvas.Children.Add(line);
        }

        private bool IsVertical(VectorModel vector)
        {
            if (vector.Start.x == vector.End.x)
                return false;
            return true;
        }

        private void AddBlock(int x, int y)//, Type type)
        {
            Rectangle rect = new Rectangle();
            rect.Stroke = new SolidColorBrush(Colors.Red);
            rect.Width = _rectangleWidth;
            rect.Height = _rectangleHeight;
            rect.StrokeThickness = _lineThickness;

            myCanvas.Children.Add(rect);
            Canvas.SetTop(rect, y * _multiplier + 20);
            Canvas.SetLeft(rect, x * _multiplier + 20);
        }
    }
}
