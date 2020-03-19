using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class EndingNode : INode
    {
        private int Id { get; set; }
        private Coordinates Position { get; set; }
        public EndingNode(int id)
        {
            this.Id = id;
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
    }
}
