using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    class Connection
    {
        private int FirstNodeId { get; set; }
        private int SecondNodeId { get; set; }

        public Connection(int firstNodeId, int secondNodeId)
        {
            this.FirstNodeId = firstNodeId;
            this.SecondNodeId = secondNodeId;
        }

        public int GetFirstNodeId()
        {
            return this.FirstNodeId;
        }

        public int GetSecondNodeId()
        {
            return this.SecondNodeId;
        }
    }
}
