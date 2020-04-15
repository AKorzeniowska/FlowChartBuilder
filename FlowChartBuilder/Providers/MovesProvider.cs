using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Providers
{
    public static class MovesProvider
    {
        public static int[][] DownRight =
        {
                new int[] { 1, 0 },
                new int[] { 0, 1 },
                new int[] { 0, -1 },
                new int[] { -1, 0 }
        };

        public static int[][] DownLeft =
        {
                new int[] { 1, 0 },
                new int[] { 0, -1 },
                new int[] { 0, 1 },
                new int[] { -1, 0 }
        };

        public static int[][] UpRight =
        {
                new int[] { -1, 0 },
                new int[] { 0, 1 },
                new int[] { 0, -1 },
                new int[] { 1, 0 }
        };

        public static int[][] UpLeft =
        {
                new int[] { -1, 0 },
                new int[] { 0, -1 },
                new int[] { 0, 1 },
                new int[] { 1, 0 }
        };

        public static int[][] RightDown =
        {
                new int[] { 0, 1 },
                new int[] { 1, 0 },
                new int[] { 0, -1 },
                new int[] { -1, 0 }
        };

        public static int[][] LeftDown =
        {
                new int[] { 0, -1 },
                new int[] { 1, 0 },
                new int[] { 0, 1 },
                new int[] { -1, 0 }
        };

        public static int[][] RightUp =
        {
                new int[] { 0, 1 },
                new int[] { -1, 0 },
                new int[] { 0, -1 },
                new int[] { 1, 0 }
        };

        public static int[][] LeftUp =
        {
                new int[] { 0, -1 },
                new int[] { -1, 0 },
                new int[] { 0, 1 },
                new int[] { 1, 0 }
        };
    }
}
