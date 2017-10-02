using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TranslationDemo : MonoBehaviour {
    
    public Vector3 translationRange;
    public Vector3 translationSpeed;

    private float xDelta = 0;
    private float yDelta = 0;
    private float zDelta = 0;

    private int xDir = 1;
    private int yDir = 1;
    private int zDir = 1;
	
	// Update is called once per frame
	void Update ()
    {
        Vector3 newPos = transform.position;

        if ((xDir == 1 && xDelta < translationRange.x) || (xDir == -1 && xDelta > -translationRange.x))
        {
            float add = translationSpeed.x * Time.deltaTime * xDir;
            xDelta += add;
            newPos.x += add;
        }
        else
        {
            xDir *= -1;
        }

        if ((yDir == 1 && yDelta < translationRange.y) || (yDir == -1 && yDelta > -translationRange.y))
        {
            float add = translationSpeed.y * Time.deltaTime * yDir;
            yDelta += add;
            newPos.y += add;
        }
        else
        {
            yDir *= -1;
        }

        if ((zDir == 1 && zDelta < translationRange.z) || (zDir == -1 && zDelta > -translationRange.z))
        {
            float add = translationSpeed.z * Time.deltaTime * zDir;
            zDelta += add;
            newPos.z += add;
        }
        else
        {
            zDir *= -1;
        }

        transform.position = newPos;
	}
}
