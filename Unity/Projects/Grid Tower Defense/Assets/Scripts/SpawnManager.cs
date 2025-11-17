using System.Collections;
using UnityEngine;
using UnityEngine.Splines;
using UnityEngine.UI;
using System.Collections.Generic;

public class SpawnManager : MonoBehaviour
{
    [SerializeField] Enemy[] enemyTypes = new Enemy[5];

    public SplineContainer spline;
    float spawnDelay = 1f;

    public Button startWaveButton;

    List<Enemy> enemiesToSpawn = new List<Enemy>();
    List<Enemy> enemiesInScene = new List<Enemy>();

    bool checkForRoundEnd = false;

    public int roundNumber = 1;
    [SerializeField] GameManager gameManager;

    public void NextRound()
    {
        StartCoroutine(SpawnEnemies());
        startWaveButton.gameObject.SetActive(false);
        gameManager.UpdateRoundNumber(roundNumber);    
    }

    public void EndRound()
    {
        roundNumber++;

        if (roundNumber > 30)
        {
            gameManager.GameWon();
            return;
        }

        startWaveButton.gameObject.SetActive(true);
        
        DetermineEnemiesToSpawn();
    }

    private void Start()
    {
        DetermineEnemiesToSpawn();
    }

    private void DetermineEnemiesToSpawn()
    {
        enemiesToSpawn.Clear();

        switch (roundNumber)
        {
            case 1:
                {
                    for (int i = 0; i < 5; i++)
                    {
                        enemiesToSpawn.Add(enemyTypes[0]);
                    }

                    break;
                }

            case int n when n > 1 && n < 5:
                {
                    for (int i = 0; i < 5 + roundNumber; i++)
                    {
                        enemiesToSpawn.Add(enemyTypes[0]);
                    }

                    break;
                }

            case 5:
                {
                    for (int i = 0; i < 5; i++)
                    {
                        enemiesToSpawn.Add(enemyTypes[1]);
                    }

                    spawnDelay = 0.9f;

                    break;
                }

            case int n when n > 5 && n < 10:
                {
                    for (int i = 0; i < 5 + roundNumber; i++)
                    {
                        enemiesToSpawn.Add(enemyTypes[Random.Range(0,2)]);
                    }

                    break;
                }

            case 10:
                {
                    for (int i = 0; i < 10; i++)
                    {
                        enemiesToSpawn.Add(enemyTypes[2]);
                    }

                    spawnDelay = 0.8f;

                    break;
                }

            case int n when n > 10 && n < 15:
            {
                    for (int i = 0; i < 5 + roundNumber; i++)
                    {
                        enemiesToSpawn.Add(enemyTypes[Random.Range(0, 3)]);
                    }

                    break;
            }

            case 15:
                {
                    for (int i = 0; i < 7; i++)
                    {
                        enemiesToSpawn.Add(enemyTypes[3]);
                    }

                    spawnDelay = 0.7f;

                    break;
                }

            case int n when n > 15 && n < 20:
                {
                    for (int i = 0; i < roundNumber; i++)
                    {
                        enemiesToSpawn.Add(enemyTypes[Random.Range(1, 4)]);
                    }

                    break;
                }

            case 20:
                {
                    enemiesToSpawn.Add(enemyTypes[4]);

                    spawnDelay = 0.6f;

                    break;
                }

            case int n when n >= 20 && n < 30:
                {
                    for (int i = 0; i < roundNumber; i++)
                    {
                        enemiesToSpawn.Add(enemyTypes[Random.Range(1, 4)]);
                    }

                    enemiesToSpawn.Add(enemyTypes[4]);

                    break;
                }

            case 30:
                {
                    for (int i = 0; i < 5; i++)
                    {
                        for (int j = 0; j < 6; j++)
                        {
                            enemiesToSpawn.Add(enemyTypes[i]);
                        }
                    }

                    spawnDelay = 0.5f;

                    break;
                }

        }

        
    }

    IEnumerator SpawnEnemies()
    {
        foreach (Enemy enemy in enemiesToSpawn)
        {
            yield return new WaitForSeconds(spawnDelay);
            Enemy newEnemy = Instantiate(enemy, new Vector3(100, 100, 100), Quaternion.identity);

            newEnemy.spline = spline;
            newEnemy.gameManager = gameManager;
            newEnemy.spawnManager = this;

            enemiesInScene.Add(newEnemy);
        }

        checkForRoundEnd = true;
    }

    private void Update()
    {
        if (checkForRoundEnd && enemiesInScene.Count == 0)
        {
            EndRound();
            checkForRoundEnd = false;
        }
    }

    public void RemoveSelfFromList(Enemy enemyToRemove)
    {
        enemiesInScene.Remove(enemyToRemove);
    }
}
