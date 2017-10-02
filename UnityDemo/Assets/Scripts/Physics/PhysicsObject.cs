using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class PhysicsObject : MonoBehaviour
{
    public Texture2D physicsTexture;
    public float minMass = 0;
    public float maxMass = 1;
    public float minStiffness = 0;
    public float maxStiffness = 1;
    public float minDamping = 0;
    public float maxDamping = 1;

    private int[] indexes;
    public int[] ObjectIndexes
    {
        set { indexes = value;}
        get { return indexes; }
    }

    private bool initialValidationDone = false;

    void OnValidate()
    {
        if (minMass      < 0) minMass      = 0;
        if (maxMass      < 0) maxMass      = minMass;
        if (minStiffness < 0) minStiffness = 0;
        if (maxStiffness < 0) maxStiffness = minStiffness;
        if (minDamping   < 0) minDamping   = 0;
        if (maxDamping   < 0) maxDamping   = minDamping;

        if (minMass      > maxMass)      minMass      = maxMass;
        if (minStiffness > maxStiffness) minStiffness = maxStiffness;
        if (minDamping   > maxDamping)   minDamping   = maxDamping;

        if (!Application.isPlaying) return;
        if (!initialValidationDone)
        {
            initialValidationDone = true;
            return;
        }
        
        if(physicsTexture != null) PhysicsSimulation.ChangePhysicsTexture(physicsTexture, indexes);
        PhysicsSimulation.ChangeObjectsPhysicsVariableLimits(minMass, maxMass, minStiffness, maxStiffness, minDamping, maxDamping, indexes);
    }
}
