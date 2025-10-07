using System;
using System.Collections.Generic;
using UnityEngine;

namespace UnityEditor.U2D.Sprites
{
    class SpriteFrameScenePreview
    {
        class SceneSprite
        {
            public readonly SpriteRenderer spriteRenderer;
            public readonly Sprite originalSprite;
            public readonly Sprite overrideSprite;

            public SceneSprite(SpriteRenderer spriteRenderer, Sprite originalSprite, Sprite overrideSprite)
            {
                this.spriteRenderer = spriteRenderer;
                this.originalSprite = originalSprite;
                this.overrideSprite = overrideSprite;
            }
        }

        SpriteRect m_SelectedSpriteRect;

        ISpriteEditor m_SpriteEditor;
        List<SceneSprite> m_SceneViewSpriteRenderers = new List<SceneSprite>();
        GameObject[] m_GameObjects;

        public SpriteFrameScenePreview(ISpriteEditor spriteEditor)
        {
            m_SpriteEditor = spriteEditor;
        }

        public void ActivateScenePreview()
        {
            m_SpriteEditor.SetScenePreviewCallback(ScenePreviewCallback);
            m_SpriteEditor.GetMainVisualContainer().RegisterCallback<SpriteSelectionChangeEvent>(SelectionChange);
            m_SpriteEditor.GetDataProvider<ISpriteEditorDataProvider>().RegisterDataChangeCallback(OnSpriteRectDataChanged);
            m_SelectedSpriteRect = m_SpriteEditor.selectedSpriteRect;
        }

        void OnSpriteRectDataChanged(ISpriteEditorDataProvider obj)
        {
            PreviewSelected();
        }

        void SelectionChange(SpriteSelectionChangeEvent evt)
        {
            m_SelectedSpriteRect = m_SpriteEditor.selectedSpriteRect;
            PreviewSelected();
        }

        void ScenePreviewCallback(GameObject[] gameObjects)
        {
            m_GameObjects = gameObjects;
            PreviewSelected();
        }

        public void DeactivateScenePreview()
        {
            RestoreSceneViewSpriteRendererSprite();
            m_SpriteEditor.SetScenePreviewCallback(null);
            m_SpriteEditor.GetMainVisualContainer().UnregisterCallback<SpriteSelectionChangeEvent>(SelectionChange);
        }

        void RestoreSceneViewSpriteRendererSprite()
        {
            if (m_SceneViewSpriteRenderers != null)
            {
                foreach (var sceneSprite in m_SceneViewSpriteRenderers)
                {
                    if (sceneSprite.spriteRenderer == null)
                        continue;

                    var currentSprite = sceneSprite.spriteRenderer.sprite;
                    if (currentSprite != sceneSprite.originalSprite && currentSprite == sceneSprite.overrideSprite)
                        sceneSprite.spriteRenderer.sprite = sceneSprite.originalSprite;
                    if(sceneSprite.overrideSprite != null)
                        UnityEngine.Object.DestroyImmediate(sceneSprite.overrideSprite);
                }

                m_SceneViewSpriteRenderers.Clear();
            }
        }

        void PreviewSelected()
        {
            RestoreSceneViewSpriteRendererSprite();
            var selectedSprite = m_SelectedSpriteRect;
            if (selectedSprite == null)
            {
                return;
            }

            if (m_GameObjects == null || m_GameObjects.Length == 0)
            {
                return;
            }

            var spriteRenderer = m_GameObjects[0].GetComponentInChildren<SpriteRenderer>();
            if (spriteRenderer == null)
            {
                return;
            }

            if (spriteRenderer.sprite == null)
            {
                return;
            }


            var originalSprite = spriteRenderer.sprite;
            // TODO there is an optimzation here where we cna reuse the same Sprite by overriding the geometry only.
            var overrideTexture = m_SpriteEditor.GetDataProvider<ITextureDataProvider>().texture;
            if (overrideTexture == null)
            {
                RestoreSceneViewSpriteRendererSprite();
                return;
            }

            // handle rect when selected rect is bigger than original texture
            var scale = 1;//overrideTexture.width / (float)m_Controller.imageSize.x;
            var rect = selectedSprite.rect;
            rect = new Rect(rect.x * scale, rect.y * scale, rect.width * scale, rect.height * scale);
            var pivot = SpriteEditorUtility.GetPivotValue(selectedSprite.alignment, selectedSprite.pivot);
            var ppu = (selectedSprite.rect.width / originalSprite.rect.width) * originalSprite.pixelsPerUnit * scale;
            var overrideSprite = Sprite.Create(overrideTexture, rect,
                pivot,
                ppu, 0,
                SpriteMeshType.FullRect);

            overrideSprite.name = $"2D-SpritePreview {GUID.Generate().ToString()}";
            m_SceneViewSpriteRenderers.Add(new SceneSprite(spriteRenderer, originalSprite, overrideSprite));
            spriteRenderer.sprite = overrideSprite;
        }
    }
}
