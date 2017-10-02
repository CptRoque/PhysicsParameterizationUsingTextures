using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScaleDemo : MonoBehaviour
{
    public Vector3 scaleRange;
    public Vector3 scaleSpeed;

    private float xDelta = 0;
    private float yDelta = 0;
    private float zDelta = 0;

    private int xDir = 1;
    private int yDir = 1;
    private int zDir = 1;

    // Update is called once per frame
    void Update()
    {
        Vector3 newScl = transform.localScale;

        if ((xDir == 1 && xDelta < scaleRange.x) || (xDir == -1 && xDelta > -scaleRange.x))
        {
            float add = scaleSpeed.x * Time.deltaTime * xDir;
            xDelta += add;
            newScl.x += add;
        }
        else
        {
            xDir *= -1;
        }

        if ((yDir == 1 && yDelta < scaleRange.y) || (yDir == -1 && yDelta > -scaleRange.y))
        {
            float add = scaleSpeed.y * Time.deltaTime * yDir;
            yDelta += add;
            newScl.y += add;
        }
        else
        {
            yDir *= -1;
        }

        if ((zDir == 1 && zDelta < scaleRange.z) || (zDir == -1 && zDelta > -scaleRange.z))
        {
            float add = scaleSpeed.z * Time.deltaTime * zDir;
            zDelta += add;
            newScl.z += add;
        }
        else
        {
            zDir *= -1;
        }
        
        transform.localScale = newScl;
    }
}
