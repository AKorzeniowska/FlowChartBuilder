using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class VectorMap
    {
        private Dictionary<int, List<VectorModel>> GroupedVectors { get; set; }
        public VectorMap()
        {
            this.GroupedVectors = new Dictionary<int, List<VectorModel>>();
        }
        public VectorMap(IEnumerable<LineModel> lines, int multiplier)
        {
            this.GroupedVectors = new Dictionary<int, List<VectorModel>>();

            int idIncrementor = 1;
            foreach (var line in lines)
            {
                var Vectors = new List<VectorModel>();
                var points = line.GetPointsOfLine();
                for (int i = 0; i < points.Count - 1; i++)
                {
                    Vectors.Add(new VectorModel(new Coordinates(points[i].x * multiplier, points[i].y * multiplier), new Coordinates(points[i + 1].x * multiplier, points[i + 1].y * multiplier)));
                }
                this.GroupedVectors.Add(idIncrementor, Vectors);
                idIncrementor++;
            }
        }

        public List<VectorModel> GetVectorsById(int id)
        {
            return GroupedVectors[id];
        }

        public void MoveAllVectorsWithId(int id, int moveX, int moveY)
        {
            foreach (var vector in GroupedVectors[id])
            {
                vector.End.x += moveX;
                vector.End.y += moveY;
                vector.Start.x += moveX;
                vector.Start.y += moveY;
            }
        }

        public Dictionary<int, List<VectorModel>> GetGroupedVectors()
        {
            return this.GroupedVectors;
        }

        public List<VectorModel> GetAllVectors()
        {
            //return this.GroupedVectors.Values.SelectMany(x => x).ToList();
            var output = new List<VectorModel>();
            foreach (var vectorlist in this.GroupedVectors)
            {
                foreach(var vec in vectorlist.Value)
                {
                    vec.Id = vectorlist.Key;
                    output.Add(vec);
                }
            }

            return output;
        }
    }
}
