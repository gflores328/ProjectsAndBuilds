using UnityEngine;

public class GridCell
{
    public Vector2Int coridinates;
    public bool occupied = false;

    public GridCell(Vector2Int cords)
    {
        coridinates = cords;
    }
}
