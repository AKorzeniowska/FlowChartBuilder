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

        public static int[][] GetReversedMoves(int[][] moves)
        {
            if (moves == DownRight) return LeftUp;
            if (moves == DownLeft) return RightUp;
            if (moves == UpRight) return LeftDown;
            if (moves == UpLeft) return RightDown;
            if (moves == RightDown) return UpLeft;
            if (moves == RightUp) return DownLeft;
            if (moves == LeftDown) return UpRight;
            if (moves == LeftUp) return DownRight;
            return DownRight;
        }
    }
}
