using FlowChartBuilder.Models;
using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder
{
    public interface INode
    {
        int GetId();
        void SetPosition(int x, int y);
        Coordinates GetPosition();
    }
}
