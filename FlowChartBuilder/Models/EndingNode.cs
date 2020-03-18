using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class EndingNode : INode
    {
        private int Id { get; set; }
        public EndingNode(int id)
        {
            this.Id = id;
        }

        public int GetId()
        {
            return this.Id;
        }
    }
}
