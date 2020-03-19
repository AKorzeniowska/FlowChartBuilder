using FlowChartBuilder;
using FlowChartBuilder.Helpers;
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
        private static readonly int _rectangleWidth = 30;
        private static readonly int _rectangleHeight = 30;
        private static readonly int _lineThickness = 3;

        private List<FlowChartBuilder.Models.Line> Lines { get; set; }
        private List<INode> Nodes { get; set; } 
        public MainWindow()
        {
            InitializeComponent();

            var path = AppDomain.CurrentDomain.BaseDirectory + @"testfiles\test2.txt";
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
                    AddLine(points[i].y, points[i + 1].y, points[i].x, points[i + 1].x);
                }
            }

            foreach (var node in Nodes)
            {
                AddBlock(node.GetPosition().y, node.GetPosition().x);
            }
        }
        private void AddLine(int x1, int x2, int y1, int y2)
        {
            Line line = new Line();
            line.StrokeThickness = _lineThickness;
            line.Stroke = System.Windows.Media.Brushes.Black;
            line.X1 = x1 * 100 + _rectangleWidth / 2 + 20;
            line.X2 = x2 * 100 + _rectangleWidth / 2 + 20;
            line.Y1 = y1 * 100 + _rectangleHeight / 2 + 20;
            line.Y2 = y2 * 100 + _rectangleHeight / 2 + 20;

            myCanvas.Children.Add(line);
        }

        private void AddBlock(int x, int y)//, Type type)
        {
            Rectangle rect = new Rectangle();
            rect.Stroke = new SolidColorBrush(Colors.Red);
            rect.Width = _rectangleWidth;
            rect.Height = _rectangleHeight;
            rect.StrokeThickness = _lineThickness;

            myCanvas.Children.Add(rect);
            Canvas.SetTop(rect, y * 100 + 20);
            Canvas.SetLeft(rect, x * 100 + 20);
        }
    }
}
