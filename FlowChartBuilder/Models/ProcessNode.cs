using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class ProcessNode : INode
    {
        private String Name { get; set; }
        private int Id { get; set; }
        private int FollowingNodeId { get; set; }
        private Coordinates Position { get; set; }

        public ProcessNode(int id)
        {
            this.Id = id;
        }

        public String GetName()
        {
            return this.Name;
        }
        public void SetName(String name)
        {
            this.Name = name;
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
