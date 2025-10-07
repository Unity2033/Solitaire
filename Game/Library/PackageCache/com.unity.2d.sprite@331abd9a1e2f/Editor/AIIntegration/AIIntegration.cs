using System.Collections.Generic;
using UnityEditor.PackageManager.Requests;
using UnityEngine;

namespace UnityEditor.U2D.Sprites.AIIntegration
{
    #if UNITY_AI_ENABLED
    [InitializeOnLoad]
    #endif
    class AIIntegration
    {
        private static List<string> s_InjectPackage = new()
        {
            "com.unity.2d.enhancers"
        };

#if UNITY_AI_ENABLED
        static AIIntegration()
        {
            Install2DEnhancerPackage();
        }
#endif
        static void Install2DEnhancerPackage()
        {
            var listRequest = PackageManager.Client.List(true);
            while (!listRequest.IsCompleted)
                System.Threading.Thread.Sleep(10);
            if (listRequest.Status == PackageManager.StatusCode.Failure)
            {
                // Failed to load package list.
                // This could be due to package manager not available in a virtual player.
                return;
            }

            var existingPackages = new List<PackageManager.PackageInfo>(listRequest.Result);
            foreach (var requiredPackage in s_InjectPackage)
            {
                bool existed = false;
                foreach(var package in existingPackages)
                {
                    if (package.name == requiredPackage)
                    {
                        existed = true;
                        break;
                    }
                }

                if (existed)
                    continue;

                existed = false;
                var version = "";
                var findRequest = PackageManager.Client.Search(requiredPackage);
                while (!findRequest.IsCompleted)
                    System.Threading.Thread.Sleep(10);
                if (findRequest.Status == PackageManager.StatusCode.Success)
                {
                    foreach(var package in findRequest.Result)
                    {
                        if (package.name == requiredPackage)
                        {
                            version = package.versions.recommended;
                            if (string.IsNullOrEmpty(version))
                                version = package.versions.latestCompatible;
                            existed = true;
                            break;
                        }
                    }
                }

                if (existed)
                {
                    AddRequest addRequest;
                    if (string.IsNullOrEmpty(version))
                        addRequest = PackageManager.Client.Add(requiredPackage);
                    else
                        addRequest = PackageManager.Client.Add(string.Format("{0}@{1}", requiredPackage, version));

                    while (!addRequest.IsCompleted)
                        System.Threading.Thread.Sleep(10);
                    if (addRequest.Status == PackageManager.StatusCode.Failure)
                        Debug.LogError($"Failed to add required package {requiredPackage}");
                }
                else
                    Debug.LogError($"{requiredPackage} not found!");
            }
        }

        [MenuItem("internal:2D/2D Enhancer/Install Package")]
        static void InstallPackage()
        {
            Install2DEnhancerPackage();
        }
    }

}
