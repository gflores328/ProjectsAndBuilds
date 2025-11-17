using System.Xml.Linq;
using UnityEngine;
using UnityEngine.Splines;

public class SplineManager : MonoBehaviour
{
    [SerializeField] GridManager gridManager;
    SplineContainer spline;
    private void Awake()
    {
        spline = GetComponent<SplineContainer>();
    }

    private void Start()
    {
        for (int i = 0; i < spline.Spline.Count - 1; i++)
        {
            OccupyCellsBetweenKnots(WorldPositionOfKnot(i), WorldPositionOfKnot(i + 1));
        }
    }

    private Vector2Int WorldPositionOfKnot(int knotIndex)
    {
        Vector3 knotPosition = spline.transform.TransformPoint(spline.Spline[knotIndex].Position);

        return new Vector2Int(Mathf.RoundToInt(knotPosition.x), Mathf.RoundToInt(knotPosition.z));
    }

    private void OccupyCellsBetweenKnots(Vector2Int knotOnePosition, Vector2Int knotTwoPosition)
    {
        int xDifference = knotTwoPosition.x - knotOnePosition.x;
        int zDifference = knotTwoPosition.y - knotOnePosition.y;

        if (gridManager.Grid.TryGetValue(knotOnePosition, out GridCell cell))
        {
            cell.occupied = true;
        }

        
        if (zDifference == 0)
        {
            int direction = (int)Mathf.Sign(xDifference);
            for (int i = 1; i < Mathf.Abs(xDifference) + 1; i++)
            {
                Vector2Int nextCell = new Vector2Int(knotOnePosition.x + (i * direction), knotOnePosition.y);
                if (gridManager.Grid.TryGetValue(nextCell, out GridCell betweenCell))
                {
                    gridManager.Grid.Remove(nextCell);
                }
            }
        }
        else if (xDifference == 0)
        {
            int direction = (int)Mathf.Sign(zDifference);
            for (int i = 1; i < Mathf.Abs(zDifference) + 1; i++)
            {
                Vector2Int nextCell = new Vector2Int(knotOnePosition.x, knotOnePosition.y + (i * direction));
                if (gridManager.Grid.TryGetValue(nextCell, out GridCell betweenCell))
                {
                    gridManager.Grid.Remove(nextCell);
                }
            }
        }
    }
}
