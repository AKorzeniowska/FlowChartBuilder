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
        private Coordinates Position { get; set; }

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

        public void AddLeftNode(int nodeId)
        {
            this.LeftFollowingNodeId = nodeId;
        }

        public void AddRightNode(int nodeId)
        {
            this.RightFollowingNodeId = nodeId;
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

        public void SetPosition(int x, int y)
        {
            this.Position = new Coordinates(x, y);
        }

        public Coordinates GetPosition()
        {
            return this.Position;
        }

        public void IncreaseId()
        {
            this.Id++;
            this.LeftFollowingNodeId++;
            this.RightFollowingNodeId++;
        }

        public bool IsNodeSelfJoining()
        {
            return (this.Id == this.LeftFollowingNodeId || this.Id == this.RightFollowingNodeId);
        }
    }
}
