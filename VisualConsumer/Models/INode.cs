using FlowChartBuilder.Models;
using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder
{
    public abstract class INode
    {
        private int Id { get; set; }
        private Coordinates Position { get; set; }
        private String Name { get; set; }
        private List<Direction> OccupiedSides { get; set; }
        private bool IncomingArrow { get; set; } = false;
        private Direction IncomingSide { get; set; }

        public int GetId()
        {
            return this.Id;
        }
        public void SetId(int id)
        {
            this.Id = id;
        }
        public abstract void IncreaseId();
        public void SetPosition(int x, int y) {
            this.Position = new Coordinates(x, y);
        }
        public Coordinates GetPosition()
        {
            return this.Position;
        }
        public abstract bool IsNodeSelfJoining();
        public string GetName()
        {
            return this.Name;
        }
        public void SetName(string name)
        {
            this.Name = name;
        }

        public void AddOccupiedSide(Direction dir)
        {
            if (this.OccupiedSides == null)
                this.OccupiedSides = new List<Direction>();
            this.OccupiedSides.Add(dir);
        }

        public List<Direction> GetOccupiedSides()
        {
            return this.OccupiedSides;
        }

        public void SetIncomingArrow()
        {
            this.IncomingArrow = true;
        }
        
        public bool HasIncomingArrow()
        {
            return this.IncomingArrow;
        }

        public void SetIncomingSide(Direction dir)
        {
            this.IncomingSide = dir;
        }
        public Direction GetIncomingSide()
        {
            return this.IncomingSide;
        }
    }
}
