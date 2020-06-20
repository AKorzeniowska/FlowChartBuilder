using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class LineModel
    {
        private List<Coordinates> Points { get; set; }
        public bool IsReversed { get; set; }
        private int TargetNodeId { get; set; }
        private Direction? ArrowDirection { get; set; }

        public LineModel()
        {
            this.Points = new List<Coordinates>();
        }

        public List<Coordinates> GetPointsOfLine()
        {
            return this.Points;
        }

        public void AddPointToLine(int x, int y)
        {
            Points.Add(new Coordinates(x, y));
        }

        public void AddPointAtStartOfLine(int x, int y)
        {
            Points.Insert(0, new Coordinates(x, y, true));
        }

        public void AddPointAtEndOfLine(int x, int y)
        {
            Points.Insert(0, new Coordinates(x, y, false));
        }

        public LineModel(LineModel line, bool isReversed)
        {
            this.Points = new List<Coordinates>();
            foreach (var point in line.GetPointsOfLine())
            {
                this.Points.Add(new Coordinates(point.x, point.y));
            }
            this.IsReversed = isReversed;
        }
        public LineModel(LineModel line)
        {
            this.Points = new List<Coordinates>();
            foreach (var point in line.GetPointsOfLine())
            {
                this.Points.Add(new Coordinates(point.x, point.y));
            }
        }

        public void DisplayLine()
        {
            foreach (var point in this.Points)
            {
                Console.WriteLine("(" + point.x + " , " + point.y + ")");
            }
        }

        public void RemoveMiddlePoints()
        {
            var newPoints = new List<Coordinates>();
            newPoints.Add(this.Points[0]);
            for (int i = 1; i < this.Points.Count - 1; i++)
            {
                var prevPoint = this.Points[i - 1];
                var point = this.Points[i];
                var nextPoint = this.Points[i + 1];
                if (prevPoint.x != nextPoint.x && prevPoint.y != nextPoint.y)
                    newPoints.Add(point);
            }
            newPoints.Add(this.Points[this.Points.Count - 1]);
            this.Points = newPoints;
        }

        public int GetTargetNodeId()
        {
            return this.TargetNodeId;
        }

        public void SetTargetNodeId(int id)
        {
            this.TargetNodeId = id;
        }

        public Direction? GetArrowDirection()
        {
            return this.ArrowDirection;
        }
        public void SetArrowDirection(Direction? dir)
        {
            this.ArrowDirection = dir;
        }
    }

    public enum Direction
    {
        S,
        W,
        E, 
        N
    }
}
