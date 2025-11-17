using System.Collections.Generic;
using NUnit.Framework;
using UnityEngine;
using UnityEngine.EventSystems;

public class GhostObject : MonoBehaviour
{
    private Renderer[] mRenderers;
    private List<Material> materials = new List<Material>();

    private GridManager gridManager;
    private GameManager gameManager;

    public BaseTower nonGhostTower;

    public TowerRangeDrawer towerRangeDrawer;

    private Vector3 currentCellOver;

    private void Awake()
    {
        towerRangeDrawer.SetTowerData(nonGhostTower.GetComponent<BaseTower>().baseTowerData);
    }

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        mRenderers = GetComponentsInChildren<Renderer>();

        foreach (Renderer renderer in mRenderers)
        {
            materials.Add(renderer.material);
        }

        gridManager = FindFirstObjectByType<GridManager>();
        gameManager = FindFirstObjectByType<GameManager>();
        
        towerRangeDrawer.shouldHideAtStart = false;
    }

    // Update is called once per frame
    void Update()
    {
        if (!gridManager)
        {
            gridManager = FindFirstObjectByType<GridManager>();
        }

        if(gridManager.GetGridPositionAtMousePosition(out Vector3 gridPosition))
        {
            currentCellOver = gridPosition;

            gameObject.transform.position = gridManager.GetPositionForTowerSize(gridPosition, nonGhostTower);
            gameObject.transform.rotation = new Quaternion(0, 0, 0,0);

            SetRenderersEnabled(true);

            bool isCellAvaiable = GetIfCellsNeededAvailable();

            if (Input.GetMouseButtonDown(0) && isCellAvaiable && !EventSystem.current.IsPointerOverGameObject())
            {
                gameManager.PlaceTower(nonGhostTower, gameObject.transform.position, gridPosition);

                Destroy(gameObject);
            }
        }
        else
        {
            SetRenderersEnabled(false);          
        }

        if(Input.GetMouseButtonDown(1))
        {
            Destroy(gameObject);
            gameManager.canSpawnTower = true;
        }
    }

    private bool GetIfCellsNeededAvailable()
    {
        if (gridManager.CanTowerPlaced(nonGhostTower, new Vector2Int(Mathf.RoundToInt(currentCellOver.x), Mathf.RoundToInt(currentCellOver.z))))
        {
            MakeTransparent(Color.green);
            return true;
        }
        else
        {
            MakeTransparent(Color.red);
            return false;
        }
    }

    bool GetIfCellAvailable()
    {
        if (gridManager.Grid.TryGetValue(new Vector2Int(Mathf.RoundToInt(transform.position.x), Mathf.RoundToInt(transform.position.z)), out GridCell thisGridCell))
        {
            MakeTransparent(Color.green);
            return true;
        }
        else 
        {
            MakeTransparent(Color.red);
            return false;
        }
    }

    void MakeTransparent(Color color)
    {
        color.a = 0.5f;
        //Material mat = mRenderer.material; 

        foreach (Material mat in materials)
        {

            mat.SetFloat("_Surface", 1);
            mat.SetFloat("_Blend", 0);
            mat.SetFloat("_ZWrite", 0);
            mat.SetOverrideTag("RenderType", "Transparent");

            mat.SetInt("_SrcBlend", (int)UnityEngine.Rendering.BlendMode.SrcAlpha);
            mat.SetInt("_DstBlend", (int)UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha);

            mat.DisableKeyword("_ALPHATEST_ON");
            mat.DisableKeyword("_ALPHAPREMULTIPLY_ON");
            mat.DisableKeyword("_ALPHAMODULATE_ON");
            mat.EnableKeyword("_SURFACE_TYPE_TRANSPARENT");
            mat.DisableKeyword("_SURFACE_TYPE_OPAQUE");

            mat.renderQueue = (int)UnityEngine.Rendering.RenderQueue.Transparent;



            mat.color = color;
        }
    }

    void SetRenderersEnabled(bool enabled)
    {
        foreach (Renderer r in mRenderers)
        {
            r.enabled = enabled;
        }
    }
}
