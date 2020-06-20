using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class DecisionNode : INode
    { 
        private int LeftFollowingNodeId { get; set; }
        private string LeftFollowingNodeCondition { get; set; }
        private int RightFollowingNodeId { get; set; }
        private string RightFollowingNodeCondition { get; set; }

        public DecisionNode(int id)
        {
            this.SetId(id);
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

        public void AddLeftNode(int nodeId)
        {
            this.LeftFollowingNodeId = nodeId;
        }

        public void AddRightNode(int nodeId)
        {
            this.RightFollowingNodeId = nodeId;
        }

        public int GetLeftFollowingNodeId()
        {
            return this.LeftFollowingNodeId;
        }
        public int GetRightFollowingNodeId()
        {
            return this.RightFollowingNodeId;
        }


        public override void IncreaseId()
        {
            var currentId = this.GetId();
            this.SetId(currentId++);
            this.LeftFollowingNodeId++;
            this.RightFollowingNodeId++;
        }

        public override bool IsNodeSelfJoining()
        {
            return (this.GetId() == this.LeftFollowingNodeId || this.GetId() == this.RightFollowingNodeId);
        }
    }
}
