using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class StartingNode : INode
    {
        private int FollowingNodeId { get; set; }
        public StartingNode(int id)
        {
            this.SetId(id);
        }
        public void AddFollowingNode(int nodeId)
        {
            this.FollowingNodeId = nodeId;
        }
        public int GetFollowingNodeId()
        {
            return this.FollowingNodeId;
        }
        public override void IncreaseId()
        {
            var currentId = this.GetId();
            this.SetId(currentId++);
            this.FollowingNodeId++;
        }

        public override bool IsNodeSelfJoining()
        {
            return this.GetId() == this.FollowingNodeId;
        }
    }
}
