using UnityEngine;

public class CellRemover : MonoBehaviour
{
    public GridManager gridManager;

    private void Start()
    {
        Vector2Int position = new Vector2Int(Mathf.RoundToInt(transform.position.x), Mathf.RoundToInt(transform.position.z));

        gridManager.RemoveCellFromData(position);

        Destroy(gameObject);
    }
}
