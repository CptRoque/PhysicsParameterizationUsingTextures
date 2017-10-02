using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RotationDemo : MonoBehaviour
{
    public Vector3 rotationRange;
    public Vector3 rotationSpeed;

    private float xDelta = 0;
    private float yDelta = 0;
    private float zDelta = 0;

    private int xDir = 1;
    private int yDir = 1;
    private int zDir = 1;

    // Update is called once per frame
    void Update()
    {
        Vector3 newRot = transform.rotation.eulerAngles;

        if ((xDir == 1 && xDelta < rotationRange.x) || (xDir == -1 && xDelta > -rotationRange.x))
        {
            float add = rotationSpeed.x * Time.deltaTime * xDir;
            xDelta += add;
            newRot.x += add;
        }
        else
        {
            xDir *= -1;
        }

        if ((yDir == 1 && yDelta < rotationRange.y) || (yDir == -1 && yDelta > -rotationRange.y))
        {
            float add = rotationSpeed.y * Time.deltaTime * yDir;
            yDelta += add;
            newRot.y += add;
        }
        else
        {
            yDir *= -1;
        }

        if ((zDir == 1 && zDelta < rotationRange.z) || (zDir == -1 && zDelta > -rotationRange.z))
        {
            float add = rotationSpeed.z * Time.deltaTime * zDir;
            zDelta += add;
            newRot.z += add;
        }
        else
        {
            zDir *= -1;
        }

        transform.eulerAngles = newRot;
    }
}
