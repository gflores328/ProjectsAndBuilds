using UnityEngine;

[ExecuteAlways]
public class Tile : MonoBehaviour
{
   [SerializeField]Vector2Int coordinates = new Vector2Int();
   GridManager gridManager;
    public bool occupied;

    private void Awake()
    {
        gridManager = FindFirstObjectByType<GridManager>();

        ChangeCoordinate();

    }

    private void Update()
    {
        ChangeCoordinate();
        transform.name = coordinates.ToString();
    }

    private void ChangeCoordinate()
    {
        if (gridManager == null)
        {
            return;
        }
        coordinates.x = Mathf.RoundToInt(transform.position.x / gridManager.UnityGridSize);
        coordinates.y = Mathf.RoundToInt(transform.position.z / gridManager.UnityGridSize);
    }

    private void OnDrawGizmos()
    {
        Gizmos.color = Color.black;
        
        if ( occupied )
        {
            Gizmos.color = Color.red;
        }

        Gizmos.DrawWireCube(transform.position, Vector3.one);
    }
}

