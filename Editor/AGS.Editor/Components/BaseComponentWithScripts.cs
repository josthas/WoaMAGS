using System;
using System.Collections.Generic;
using System.Text;
using AGS.Types;
using WeifenLuo.WinFormsUI.Docking;

namespace AGS.Editor.Components
{
    internal abstract class BaseComponentWithScripts<TScript, TScriptFolder> : BaseComponentWithFolders<TScript, TScriptFolder>
        where TScript : IToXml
        where TScriptFolder : BaseFolderCollection<TScript, TScriptFolder>
    {
        public BaseComponentWithScripts(GUIController guiController, AGSEditor agsEditor, string topLevelCommandId)
            : base(guiController, agsEditor, topLevelCommandId)
        {
        }

        protected void ZoomToCorrectPositionInScript(ScriptEditor editor, ZoomToFileEventArgs evArgs)
        {
            if (editor == null)
            {
                return;
            }

            if (evArgs.ZoomType == ZoomToFileZoomType.ZoomToCharacterPosition)
            {
                editor.GoToLineOfCharacterPosition(evArgs.ZoomPosition, evArgs.SelectLine);
            }
            else if (evArgs.ZoomType != ZoomToFileZoomType.DoNotMoveCursor)
            {
                if (evArgs.ZoomToText != null)
                {
                    evArgs.ZoomPosition = editor.GetLineNumberForText(evArgs.ZoomToText);
                }
                editor.GoToLine(evArgs.ZoomPosition, evArgs.SelectLine, evArgs.ZoomToLineAfterOpeningBrace);

                if (evArgs.IsDebugExecutionPoint)
                {
                    editor.SetExecutionPointMarker(evArgs.ZoomPosition);
                    if (evArgs.ErrorMessage != null)
                    {
                        editor.SetErrorMessagePopup(evArgs.ErrorMessage);
                    }
                }
            }
        }

        protected abstract ContentDocument GetDocument(ScriptEditor editor);

        protected void ScriptEditor_IsModifiedChanged(object sender, EventArgs e)
        {
            ScriptEditor sendingPane = (ScriptEditor)sender;
            UpdateScriptWindowTitle(sendingPane);
        }

        protected void ScriptEditor_DockStateChanged_Hack(object sender, EventArgs e)
        {
            ScriptEditor editor = (ScriptEditor)sender;
            ReInitializeScriptEditor(editor); 
        }

        protected void ScriptEditor_DockStateChanged(object sender, EventArgs e)
        {
            DockingContainer container = (DockingContainer)sender;
            ScriptEditor editor = (ScriptEditor)container.Panel;
            if (container.DockState == DockingState.Hidden) return;            
            container.InitScriptIfNeeded<ScriptEditor>(ReInitializeScriptEditor, editor);            
        }

        protected void UpdateScriptWindowTitle(ScriptEditor editor)
        {
            string newTitle = editor.Script.FileName + (editor.IsModified ? " *" : "");
            ContentDocument document = GetDocument(editor);            
            if (document != null && document.Name != newTitle)
            {
                document.Name = newTitle;
                document.Control.DockingContainer.Text = newTitle;
                _guiController.DocumentTitlesChanged();
            }            
        }
        
        protected virtual void ReInitializeScriptEditor(ScriptEditor editor)
        {
            Script script = editor.Script;
            string modifiedText = editor.ModifiedText;
            if (modifiedText != null && 
                modifiedText.Equals(script.Text))
            {
                modifiedText = null;
            }
            editor.Clear();
            editor.Init(script);
            editor.ActivateWindow();
            if (modifiedText != null)
            {
                editor.ModifiedText = modifiedText;
            }
            UpdateScriptWindowTitle(editor);
        }
    }
}
