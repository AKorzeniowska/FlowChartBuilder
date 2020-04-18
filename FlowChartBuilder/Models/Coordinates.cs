using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class Coordinates
    {
        public int x { get; set; }
        public int y { get; set; }
        public Boolean? AtStart { get; set; }

        public Coordinates(int x, int y)
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
    }
}
