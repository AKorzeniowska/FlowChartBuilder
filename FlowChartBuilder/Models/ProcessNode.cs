using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class ProcessNode : INode
    {
        private int Id { get; set; }
        private int FollowingNodeId { get; set; }

        public ProcessNode(int id)
        {
            this.Id = id;
        }

        public void AddFollowingNode(int nodeId)
        {
            this.FollowingNodeId = nodeId;
        }

        public int GetId()
        {
            return this.Id;
        }

        public int GetFollowingNodeId()
        {
            return this.FollowingNodeId;
        }
    }
}
