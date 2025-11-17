using UnityEngine;
using UnityEngine.Rendering;

public class TowerRangeDrawer : MonoBehaviour
{
    int segments = 100;

    LineRenderer lineRenderer;

    public BaseTowerData ownerData;

    public bool shouldHideAtStart = true;

    private void Awake()
    {
        lineRenderer = gameObject.GetComponent<LineRenderer>();
        lineRenderer.useWorldSpace = false;
        lineRenderer.startWidth = 0.1f;
    }

    private void Start()
    {
        if (shouldHideAtStart)
        {
            HideRange();
        }
    }

    private void EstablishRange()
    {
        lineRenderer.positionCount = segments + 1;

        for (int i = 0; i <= segments; i++)
        {
            float angle = i * (360f / (float)segments);
            float x = Mathf.Sin(Mathf.Deg2Rad  * angle) * ownerData.baseRange;
            float z = Mathf.Cos(Mathf.Deg2Rad * angle) * ownerData.baseRange;

            lineRenderer.SetPosition(i, new Vector3(x, 0, z));
        }
    }

    public void ShowRange()
    {
        lineRenderer.enabled = true;
    }

    public void HideRange()
    {
        lineRenderer.enabled = false;
    }

    public void SetTowerData(BaseTowerData towerData)
    {
        ownerData = towerData;

        if (lineRenderer == null)
        {
            lineRenderer = GetComponent<LineRenderer>();
            lineRenderer.useWorldSpace = false;
            lineRenderer.startWidth = 0.1f;
        }

        EstablishRange(); 
    }
}
