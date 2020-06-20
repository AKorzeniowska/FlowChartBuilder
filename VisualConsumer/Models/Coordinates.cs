using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class Coordinates
    {
        public double x { get; set; }
        public double y { get; set; }
        public Boolean? AtStart { get; set; }

        public Coordinates(int x, int y)
        {
            this.x = x;
            this.y = y;
            this.AtStart = null;
        }

        public Coordinates(double x, double y)
        {
            this.x = x;
            this.y = y;
            this.AtStart = null;
        }

        public Coordinates(int x, int y, bool atStart)
        {
            this.x = x;
            this.y = y;
            this.AtStart = atStart;
        }

        public override bool Equals(object obj)
        {
            if (obj.GetType() != typeof(Coordinates)) return false;
            var coords = (Coordinates)obj;
            if (coords.x == this.x && coords.y == this.y) return true;
            return false;
        }
    }
}
