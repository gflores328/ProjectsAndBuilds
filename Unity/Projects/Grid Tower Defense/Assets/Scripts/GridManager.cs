using System;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.Experimental.GlobalIllumination;

[ExecuteAlways]
public class GridManager : MonoBehaviour
{
    [SerializeField] Vector2Int gridSize;
    [SerializeField] int unityGridSize;
    public int UnityGridSize { get { return unityGridSize; } }

    Dictionary<Vector2Int, GridCell> grid = new Dictionary<Vector2Int, GridCell>();
    public Dictionary<Vector2Int, GridCell> Grid { get { return grid; } }

    Color lineColor = Color.aquamarine;


    private void Awake()
    {
        UpdateGrid();
    }

    [ContextMenu("Update Grid")]
    private void UpdateGrid()
    {
        grid.Clear();

        for (int x = 0; x < gridSize.x; x++)
        {
            for (int y = 0; y < gridSize.y; y++)
            {
                Vector2Int cordinates = new Vector2Int(x + 1, y + 1 );
                grid.Add(cordinates, new GridCell(cordinates));
            }
        }
    }

    private void OnDrawGizmos()
    {
        Gizmos.color = Color.green;

        for (int x = 0; x < gridSize.x; x++)
        {
            for (int y = 0; y < gridSize.y; y++)
            {
                Vector3 center = transform.position + new Vector3((x+1) * unityGridSize + unityGridSize / 2, 0, y + unityGridSize + unityGridSize / 2);
                Gizmos.DrawWireCube(center, new Vector3(unityGridSize, 0, unityGridSize));
            }
        }
    }

    [ContextMenu("Test Grid Coordinates")]
    void Test()
    {
        for (int x = 0; x < gridSize.x; x++)
        {
            for (int y = 0; y < gridSize.y; y++)
            {
                Debug.Log($"{grid[new Vector2Int(x + 1,y + 1)].coridinates}");
            }
        }
    }

    public bool GetGridPositionAtMousePosition(out Vector3 cellPosition)
    {
        cellPosition = Vector3.zero;
        RaycastHit hit;
        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
        LayerMask layerToIgnore = LayerMask.NameToLayer("Tower");

        if (Physics.Raycast(ray, out hit, 100f, layerToIgnore))
        {
            Vector3 hitPoint = new Vector3(Mathf.Round(hit.point.x), hit.point.y, Mathf.Round(hit.point.z));
            cellPosition = hitPoint;
            return true;
        }
        return false;
    }

    public bool CanTowerPlaced(BaseTower towerToCheck, Vector2Int startingCell)
    {
        if (towerToCheck.xSize == 1 && towerToCheck.zSize == 1)
        {
            return Grid.TryGetValue(startingCell, out GridCell cell);
        }

        for (int i = 0; i < towerToCheck.xSize; i++)
        {
            for (int j = 0; j < towerToCheck.zSize; j++)
            {
                if (!Grid.TryGetValue(new Vector2Int(startingCell.x + i, startingCell.y + j), out GridCell cell))
                {
                    return false;
                }
            }
        }

        return true;
    }

    public void RemoveCellFromData(Vector2Int coordinates)
    {
        grid.Remove(coordinates);
    }

    public void RemoveCellsFromData(Vector2Int startingCell, BaseTower towerSize)
    {
        for(int i = 0;i < towerSize.xSize;i++)
        {
            for (int j = 0; j < towerSize.zSize; j++)
            {
                grid.Remove(new Vector2Int(startingCell.x + i, startingCell.y + j));
            }
        }
    }

    public void AddCellToData(Vector2Int coordinates)
    {
        grid.Add(coordinates, new GridCell(coordinates));
    }

    public void AddCellsToData(Vector2Int startingCell, BaseTower towerSize)
    {
        for (int i = 0; i < towerSize.xSize; i++)
        {
            for (int j = 0; j < towerSize.zSize; j++)
            {
                Vector2Int addedCell = new Vector2Int(startingCell.x + i,startingCell.y + j);

                grid.Add(addedCell, new GridCell(addedCell));
            }
        }
    }

    public bool CheckIfCellOccupied(Vector2Int coordinates)
    {
        Grid.TryGetValue(coordinates, out GridCell checkedCell);
        return checkedCell.occupied;
    }

    public Vector3 GetPositionForTowerSize(Vector3 firstCellPosition, BaseTower towerToPlace)
    {
        Vector3 placementPosition = Vector3.zero;
        int towerCellSize = 0;

        if (towerToPlace.xSize == 1 && towerToPlace.zSize == 1)
        {
            return firstCellPosition;
        }

        for (int i = 0; i < towerToPlace.xSize; i++)
        {
            for (int j = 0; j < towerToPlace.zSize; j++)
            {
                placementPosition += new Vector3(firstCellPosition.x + i, firstCellPosition.y, firstCellPosition.z + j);
                towerCellSize++;
            }
        }

        return (placementPosition / (towerToPlace.xSize * towerToPlace.zSize));
    }

    private void OnRenderObject()
    {

        GL.PushMatrix();
        GL.MultMatrix(transform.localToWorldMatrix);

        Material lineMat = new Material(Shader.Find("Hidden/Internal-Colored"));
        lineMat.SetPass(0);

        GL.Begin(GL.LINES);
        GL.Color(lineColor);

        int sizeX = UnityGridSize * Grid.Count;
        int sizeY = UnityGridSize * Grid.Count;

        for (int x = 0; x <= gridSize.x; x++)
        {
            float xPos = (x * UnityGridSize) + UnityGridSize / 2f;
            GL.Vertex3(xPos, 0, UnityGridSize / 2f);
            GL.Vertex3(xPos, 0, (gridSize.y * UnityGridSize) + UnityGridSize / 2f);
        }

        for (int y = 0; y <= gridSize.y; y++)
        {
            float yPos = (y * UnityGridSize) + UnityGridSize / 2f;
            GL.Vertex3(UnityGridSize / 2f, 0, yPos);
            GL.Vertex3((gridSize.x * UnityGridSize) + UnityGridSize / 2f, 0, yPos);
        }

        GL.End();
        GL.PopMatrix();
    }
}
