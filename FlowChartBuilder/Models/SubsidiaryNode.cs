using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class SubsidiaryNode : INode
    {
        private int Id { get; set; }
        private Coordinates Position { get; set; }
        private int FollowingNodeId { get; set; }
        public int GetId()
        {
            return this.Id;
        }

        public int GetFollowingNodeId()
        {
            return this.FollowingNodeId;
        }

        public SubsidiaryNode(int id, int followingNodeId)
        {
            this.Id = id;
            this.FollowingNodeId = followingNodeId;
        }

        public Coordinates GetPosition()
        {
            return this.Position;
        }

        public void IncreaseId()
        {
            this.Id++;
        }

        public void SetPosition(int x, int y)
        {
            this.Position = new Coordinates(x, y);
        }

        public bool IsNodeSelfJoining()
        {
            return false;
        }

        public string GetName()
        {
            return "";
        }
    }
}
