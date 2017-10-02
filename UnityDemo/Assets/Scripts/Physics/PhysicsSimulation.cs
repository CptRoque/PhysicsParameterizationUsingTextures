using UnityEngine;
using System.Runtime.InteropServices;
using System.Collections.Generic;

public class PhysicsSimulation : MonoBehaviour
{
    private class ObjectData //has to be class to be added as reference to list, if struct the pointer to position will be changed
    {
        public int index;
        public Vector2[] uv;
        public Vector3[] vertices;
        public Color[] pixels;
        public Transform transform;
        public Vector3 position;
        public Quaternion rotation;
        public Vector3 scale;
        public Mesh mesh;
    }

    private static List<ObjectData> datas;

    [DllImport("PhysicsCore", EntryPoint = "AddObjectToSimulation")]
    private static extern int AddObjectToSimulation(ref Vector3 position, ref Quaternion rotation, ref Vector3 scale, Vector3[] meshVertices, Vector2[] meshUV, int vertexAmount, Color[] pixels, int textureHeight, int textureWidth, float minMass, float maxMass, float minStiffness, float maxStiffness, float minDamping, float maxDamping);
   [DllImport("PhysicsCore", EntryPoint = "ChangeObjectsPhysicsTexture")]
    private static extern void ChangeObjectsPhysicsTexture(int index, Color[] pixels, int textureHeight, int textureWidth);
    [DllImport("PhysicsCore", EntryPoint = "ChangeObjectsPhysicsVariableLimits")]
    private static extern void ChangeObjectsPhysicsVariableLimits(int index, float minMass, float maxMass, float minStiffness, float maxStiffness, float minDamping, float maxDamping);
    [DllImport("PhysicsCore", EntryPoint = "RemoveObjectFromSimulation")]
    private static extern void RemoveObjectFromSimulation(int index);
    [DllImport("PhysicsCore", EntryPoint = "Simulate")]
    private static extern void Simulate(float deltaTime);
    [DllImport("PhysicsCore", EntryPoint = "CleanSimulationData")]
    private static extern void CleanSimulationData();

    void Start()
    {
        datas = new List<ObjectData>();

        PhysicsObject[] objects = FindObjectsOfType(typeof(PhysicsObject)) as PhysicsObject[];
        for (int i = 0; i < objects.Length; i++)
        {
            if (objects[i].physicsTexture != null)
            {
                objects[i].ObjectIndexes = AddObject(objects[i].gameObject, objects[i].physicsTexture, objects[i].minMass, objects[i].maxMass, objects[i].minStiffness, objects[i].maxStiffness, objects[i].minDamping, objects[i].maxDamping);
            }
        }
    }

    void Update()
    {
        for (int i = 0; i < datas.Count; i++)
        {
            datas[i].position = datas[i].transform.position;
            datas[i].rotation = datas[i].transform.rotation;
            datas[i].scale = datas[i].transform.lossyScale;
            UpdateMesh(datas[i]);
        }

        PhysicsSimulation.Simulate(Time.deltaTime);

        for (int i = 0; i < datas.Count; i++)
        {
            datas[i].mesh.vertices = datas[i].vertices;
            datas[i].transform.position = datas[i].position;

        }
    }

    void OnApplicationQuit()
    {
        PhysicsSimulation.CleanSimulationData();
    }

    public static int[] AddObject(GameObject gameObject, Texture2D texture, float minMass, float maxMass, float minStiffness, float maxStiffness, float minDamping, float maxDamping)
    {
        List<int> indexes = new List<int>();
        var filters = gameObject.GetComponentsInChildren<MeshFilter>();
        if (filters != null)
        {
            foreach (var filter in filters)
            {
                filter.mesh.MarkDynamic();
                ObjectData meshData = new ObjectData();
                meshData.mesh = filter.mesh;
                meshData.vertices = meshData.mesh.vertices;
                meshData.uv = meshData.mesh.uv;
                meshData.pixels = texture.GetPixels();
                meshData.transform = filter.gameObject.transform;
                meshData.position = filter.gameObject.transform.position;
                meshData.rotation = filter.gameObject.transform.rotation;
                meshData.scale = filter.gameObject.transform.lossyScale;

                int index = PhysicsSimulation.AddObjectToSimulation(ref meshData.position, ref meshData.rotation, ref meshData.scale, meshData.vertices, meshData.uv, meshData.mesh.vertexCount, meshData.pixels, texture.height, texture.width, minMass, maxMass, minStiffness, maxStiffness, minDamping, maxDamping);
                meshData.index = index;
                datas.Add(meshData);
                indexes.Add(index);
            }
        }
        return indexes.ToArray();
    }

    public static void ChangePhysicsTexture(Texture2D newTexture, params int[] index)
    {
        Color[] colors = newTexture.GetPixels();

        int textureHeight = newTexture.height;
        int textureWidth = newTexture.width;

        for (int i = 0; i < index.Length; i++)
        {
            ObjectData oD = datas.Find(data => data.index == index[i]);
            oD.pixels = colors;
            PhysicsSimulation.ChangeObjectsPhysicsTexture(index[i], colors, textureHeight, textureWidth);
        }
    }

    public static void ChangeObjectsPhysicsVariableLimits(float minMass, float maxMass, float minStiffness, float maxStiffness, float minDamping, float maxDamping, params int[] index)
    {
        for (int i = 0; i < index.Length; i++)
        {
            PhysicsSimulation.ChangeObjectsPhysicsVariableLimits(index[i], minMass, maxMass, minStiffness, maxStiffness, minDamping, maxDamping);
        }
    }

    public static void RemoveObjectsFromSimulation(params int[] index)
    {
        for (int i = 0; i < index.Length; i++)
        {
            PhysicsSimulation.RemoveObjectFromSimulation(index[i]);
            datas.RemoveAll(data => data.index == index[i]);
        }
    }

    private void UpdateMesh(ObjectData meshData)
    {
        meshData.mesh.RecalculateNormals();
        meshData.mesh.RecalculateBounds();
    }
}
