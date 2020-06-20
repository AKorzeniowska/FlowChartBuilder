using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class VectorModel : IEquatable<VectorModel>
    {

        public VectorModel(Coordinates start, Coordinates end)
        {
            this.Start = start;
            this.End = end;
        }

        public VectorModel(Coordinates start, Coordinates end, int id)
        {
            this.Start = start;
            this.End = end;
            this.Id = id;
        }

        public Coordinates Start { get; set; }
        public Coordinates End { get; set; }
        public int Id { get; set; } = 0;
        public int ChangeNumber { get; set; }

        public bool Equals(VectorModel other)
        {
            if (this.Id != other.Id)
                return false;
            if (this.Start.x != other.Start.x || this.Start.y != other.Start.y)
                return false;
            if (this.End.x != other.End.x || this.End.y != other.End.y)
                return false;
            return true;
        }

        public int GetChangeNumber()
        {
            return this.ChangeNumber;
        }

        public void AddChange()
        {
            this.ChangeNumber++;
        }
    }
}
