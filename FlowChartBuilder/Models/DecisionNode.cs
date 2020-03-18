using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class DecisionNode : INode
    {
        private int Id { get; set; }
        private int LeftFollowingNodeId { get; set; }
        private string LeftFollowingNodeCondition { get; set; }
        private int RightFollowingNodeId { get; set; }
        private string RightFollowingNodeCondition { get; set; }

        public DecisionNode(int id)
        {
            this.Id = id;
        }

        public void AddLeftNode(int nodeId, string condition)
        {
            this.LeftFollowingNodeId = nodeId;
            this.LeftFollowingNodeCondition = condition;
        }

        public void AddRightNode(int nodeId, string condition)
        {
            this.RightFollowingNodeId = nodeId;
            this.RightFollowingNodeCondition = condition;
        }

        public int GetId()
        {
            return this.Id;
        }
        public int GetLeftFollowingNodeId()
        {
            return this.LeftFollowingNodeId;
        }
        public int GetRightFollowingNodeId()
        {
            return this.RightFollowingNodeId;
        }
    }
}
