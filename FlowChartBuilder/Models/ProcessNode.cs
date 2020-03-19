using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class ProcessNode : INode
    {
        private int Id { get; set; }
        private int FollowingNodeId { get; set; }
        private Coordinates Position { get; set; }

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

        public void SetPosition(int x, int y)
        {
            this.Position = new Coordinates(x, y);
        }

        public Coordinates GetPosition()
        {
            return this.Position;
        }
    }
}
