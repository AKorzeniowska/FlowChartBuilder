using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class VectorModel
    {

        public VectorModel(Coordinates start, Coordinates end)
        {
            this.Start = start;
            this.End = end;
        }

        public Coordinates Start { get; set; }
        public Coordinates End { get; set; }
    }
}
