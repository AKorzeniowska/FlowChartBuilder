using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class EndingNode : INode
    {
        public EndingNode(int id)
        {
            this.SetId(id);
        }

        public override void IncreaseId()
        {
            var currentId = this.GetId();
            this.SetId(currentId++);
        }

        public override bool IsNodeSelfJoining()
        {
            return false;
        }
    }
}
