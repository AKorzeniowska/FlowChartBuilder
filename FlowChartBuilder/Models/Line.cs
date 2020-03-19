﻿using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class Line
    {
        private List<Coordinates> Points { get; set; }

        public Line()
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
            Points.Insert(0, new Coordinates(x, y));
        }

        public Line(Line line)
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
    }
}
