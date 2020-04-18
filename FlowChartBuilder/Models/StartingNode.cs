using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class StartingNode : INode
    {
        private int Id { get; set; }
        private int FollowingNodeId { get; set; }
        private Coordinates Position { get; set; }
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
            this.FollowingNodeId++;
        }

        public bool IsNodeSelfJoining()
        {
            return this.Id == this.FollowingNodeId;
        }
    }
}
