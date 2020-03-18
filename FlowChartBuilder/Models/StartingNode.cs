using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class StartingNode : INode
    {
        private int Id { get; set; }
        private int FollowingNodeId { get; set; }
        public StartingNode(int id)
        {
            this.Id = id;
        }
        public void AddFollowingNode(int nodeId)
        {
            this.FollowingNodeId = nodeId;
        }
        public int GetFollowingNodeId()
        {
            return this.FollowingNodeId;
        }

        public int GetId()
        {
            return this.Id;
        }
    }
}
