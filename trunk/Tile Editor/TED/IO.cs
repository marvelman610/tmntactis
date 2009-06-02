//  [5/27/2009 Ramon Johannessen (RJ)]

/// <summary>
///  File : IO.cs
///  Purpose : A Partial class with the main Form1 that handles all IO-based operations
/// </summary>
using System;
using System.Data;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;
using System.Xml;
using System.Drawing;
using map;
using tile;

namespace test
{
    enum LOAD_TYPE{XML, BINARY, };
    // all saving and loading code in here
    /// <summary>
    /// File IO...and all that THAT encompasses...
    /// </summary>
    public partial class Form1 : Form
    {
        int loadType;

        private void asTextFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_nLastSaveType = (int)SAVE_TYPE.TEXT;
            SaveFile(false);
        }
        private void asBinaryFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_nLastSaveType = (int)SAVE_TYPE.BINARY;
            SaveFile(false);
        }
        private void xMLToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            m_nLastSaveType = (int)SAVE_TYPE.XML;
            SaveFile(false);
        }
        private void saveToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            SaveFile(m_bHaveSaved);
        }
        /// <summary>
        /// Handles binary, text, and xml saving depending on which was chosen, if
        /// save is chosen, the last file type to be saved is used, if there has been
        /// no saves, XML is default.
        /// 
        ///     <SaveParams>
        /// 
        ///     Save Variables:
        ///     Version number.
        ///     Map Name.
        ///     Is it isometric?
        ///     Total number of tiles.
        ///     Columns.
        ///     Rows.
        ///     Cell Width.
        ///     Cell Height.
        ///     "Layer Name".
        ///     
        ///         <IndividualTiles>
        ///         Source File Name.
        ///         ID (tileset tile ID).
        ///         MapID
        ///         Flag.
        ///         (Free Placed adds x and y, source rect is used for tileset x and y).
        ///         Tile Width.
        ///         Tile Height.
        ///         Trigger.
        ///         </IndividualTiles>
        ///     
        ///     </SaveParams>
        /// </summary>
        /// <param name="bHaveSaved">Has the user saved since any changes?</param>
        /// <returns>true if successful</returns>
        private bool SaveFile(bool bHaveSaved)
        {
            switch (m_nLastSaveType)
            {
                case (int)SAVE_TYPE.BINARY:
                    {
                        SaveBinary(bHaveSaved);
                        if (saveXMLWithToolStripMenuItem.Checked)
                            SaveXMLAuto();
                    }
                    break;
                case (int)SAVE_TYPE.TEXT:
                    {
                        SaveText(bHaveSaved);
                    }
                    break;
                case (int)SAVE_TYPE.XML:
                    {
                        SaveXML(bHaveSaved);
                    }
                    break;
            }
            return true;
        }

        private bool SaveText(bool bHaveSaved)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            DialogResult dr = new DialogResult();
            sfd.FileName = m_strSaveFileName + ".txt";
            sfd.Filter = "TXT Files|*.txt";
            if (!bHaveSaved)
            {
                dr = sfd.ShowDialog();
            }
            if (dr == DialogResult.Cancel)
                return false;

            if (DialogResult.OK == dr || bHaveSaved)
            {
                m_strSaveFileName = Path.GetFileNameWithoutExtension(sfd.FileName);
                this.Text = "TED - " + m_strSaveFileName;

                TextWriter tw = new StreamWriter(sfd.FileName, false);
                tw.WriteLine("TotalTiles=" + m_mMap.NTotalNumTiles.ToString());
                tw.WriteLine(m_mMap.NNumCols.ToString());
                tw.WriteLine(m_mMap.NNumRows.ToString());
                tw.WriteLine("Layer1");
                for (int i = 0; i < m_mMap.NTotalNumTiles; ++i)
                {
                    tw.WriteLine("ID\\" + m_mMap.TMapTiles[i].NSourceID.ToString());
                    tw.WriteLine("Flag\\" + m_mMap.TMapTiles[i].NTileFlag.ToString());
                }
                if (m_bIsTwoLayersOn)
                {
                    tw.WriteLine("Layer2");
                    for (int i = 0; i < m_mMap.NTotalNumTiles; ++i)
                    {
                        tw.WriteLine("ID\\" + m_mMap.TMapTilesLayer2[i].NSourceID.ToString());
                        tw.WriteLine("Flag\\" + m_mMap.TMapTilesLayer2[i].NTileFlag.ToString());
                    }

                }
                m_bChangesMade = false;
                m_bHaveSaved = true;
            }
            return true;
        }
        private bool SaveXML(bool bHaveSaved)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            DialogResult dr = new DialogResult();
            sfd.FileName = m_strSaveFileName + ".xml";
            sfd.Filter = "XML Files|*.xml";
            if (!bHaveSaved)
            {
                dr = sfd.ShowDialog();
            }
            if (dr == DialogResult.Cancel)
            {
                return false;
            }
            if (DialogResult.OK == dr || bHaveSaved)
            {
                m_bChangesMade = false;
                m_bHaveSaved = true;
                m_strSaveFileName = Path.GetFileName(sfd.FileName);
                this.Text = "TED - " + Path.GetFileNameWithoutExtension(m_strSaveFileName);

                XmlWriterSettings settings = new XmlWriterSettings();
                settings.ConformanceLevel = ConformanceLevel.Document;
                settings.Indent = true;

                using (XmlWriter xml = XmlWriter.Create(sfd.FileName, settings))
                {
                    xml.WriteStartElement(m_strVersionNumber);
                    xml.WriteStartElement("Map");
                    xml.WriteAttributeString("Name", m_strSaveFileName);
                    xml.WriteAttributeString("Isometric", m_bIsIsometric.ToString());
                    xml.WriteAttributeString("TotalTiles", m_mMap.NTotalNumTiles.ToString());
                    xml.WriteAttributeString("Columns", m_mMap.NNumCols.ToString());
                    xml.WriteAttributeString("Rows", m_mMap.NNumRows.ToString());
                    xml.WriteAttributeString("CellWidth", m_mMap.NCellWidth.ToString());
                    xml.WriteAttributeString("CellHeight", m_mMap.NCellHeight.ToString());
                    xml.WriteAttributeString("NumTilesets", (tabControl1.TabCount - 1).ToString());
                    xml.WriteEndElement();

                    for (int i = 0; i < tabControl1.TabCount - 1; ++i)
                    {
                        xml.WriteStartElement("Tileset");
                        xml.WriteAttributeString("FileName", m_strTilesetFilenames[i]);
                        xml.WriteAttributeString("ColorKeyRed", m_clrKey[i].R.ToString());
                        xml.WriteAttributeString("ColorKeyGreen", m_clrKey[i].G.ToString());
                        xml.WriteAttributeString("ColorKeyBlue", m_clrKey[i].B.ToString());
                        xml.WriteAttributeString("Width", m_tsTileset[i].NTilesetWidth.ToString());
                        xml.WriteAttributeString("Height", m_tsTileset[i].NTilesetHeight.ToString());
                        xml.WriteAttributeString("CellWidth", m_tsTileset[i].NCellWidth.ToString());
                        xml.WriteAttributeString("CellHeight", m_tsTileset[i].NCellHeight.ToString());
                        xml.WriteEndElement();
                    }

                    xml.WriteStartElement("Layer1");
                    for (int i = 0; i < m_mMap.NTotalNumTiles; ++i)
                    {
                        if (m_mMap.TMapTiles[i].NSourceID != -1)
                        {
                            xml.WriteStartElement("TILE");
                            xml.WriteElementString("MapID", i.ToString());
                            xml.WriteElementString("ImageID", m_mMap.TMapTiles[i].ImageID.ToString());
                            xml.WriteElementString("ID", m_mMap.TMapTiles[i].NSourceID.ToString());
                            xml.WriteElementString("Flag", m_mMap.TMapTiles[i].NTileFlag.ToString());
                            xml.WriteElementString("SourceX", m_mMap.TMapTiles[i].SourceRect.X.ToString());
                            xml.WriteElementString("SourceY", m_mMap.TMapTiles[i].SourceRect.Y.ToString());
                            xml.WriteElementString("Width", m_mMap.TMapTiles[i].SourceRect.Width.ToString());
                            xml.WriteElementString("Height", m_mMap.TMapTiles[i].SourceRect.Height.ToString());
                            xml.WriteElementString("Trigger", m_mMap.TMapTiles[i].Trigger);
                            xml.WriteEndElement();
                        }
                    }
                    xml.WriteEndElement();

                    xml.WriteStartElement("Layer2");
                    for (int i = 0; i < m_mMap.NTotalNumTiles; ++i)
                    {
                        if (m_mMap.TMapTilesLayer2[i].NSourceID != -1)
                        {
                            xml.WriteStartElement("TILE");
                            xml.WriteElementString("MapID", i.ToString());
                            xml.WriteElementString("ImageID", m_mMap.TMapTilesLayer2[i].ImageID.ToString());
                            xml.WriteElementString("ID", m_mMap.TMapTilesLayer2[i].NSourceID.ToString());
                            xml.WriteElementString("Flag", m_mMap.TMapTilesLayer2[i].NTileFlag.ToString());
                            xml.WriteElementString("SourceX", m_mMap.TMapTilesLayer2[i].SourceRect.X.ToString());
                            xml.WriteElementString("SourceY", m_mMap.TMapTilesLayer2[i].SourceRect.Y.ToString());
                            xml.WriteElementString("Width", m_mMap.TMapTilesLayer2[i].SourceRect.Width.ToString());
                            xml.WriteElementString("Height", m_mMap.TMapTilesLayer2[i].SourceRect.Height.ToString());
                            xml.WriteElementString("Trigger", m_mMap.TMapTilesLayer2[i].Trigger.ToString());
                            xml.WriteEndElement();
                        }
                    }
                    xml.WriteEndElement();

                    xml.WriteStartElement("FreePlace");
                    for (int i = 0; i < m_mMap.FreePlaced.GetLength(0); ++i)
                    {
                        if (m_mMap.FreePlaced[i] != null)
                        {
                            xml.WriteStartElement("FREETILE");
                            xml.WriteElementString("ImageID", m_mMap.FreePlaced[i].ImageID.ToString());
                            xml.WriteElementString("SourceX", m_mMap.FreePlaced[i].SourceRect.X.ToString());
                            xml.WriteElementString("SourceY", m_mMap.FreePlaced[i].SourceRect.Y.ToString());
                            xml.WriteElementString("Flag", m_mMap.FreePlaced[i].NTileFlag.ToString());
                            xml.WriteElementString("X", m_mMap.FreePlaced[i].MapPt.X.ToString());
                            xml.WriteElementString("Y", m_mMap.FreePlaced[i].MapPt.Y.ToString());
                            xml.WriteElementString("Width", m_mMap.FreePlaced[i].SourceRect.Width.ToString());
                            xml.WriteElementString("Height", m_mMap.FreePlaced[i].SourceRect.Height.ToString());
                            xml.WriteElementString("Trigger", m_mMap.FreePlaced[i].Trigger.ToString());

                            xml.WriteEndElement();
                        }
                    }
                    xml.WriteEndElement();
                    xml.WriteEndElement();
                }
            }
            return true;
        }
        private bool SaveXMLAuto()
        {
            m_bChangesMade = false;
            m_bHaveSaved = true;
            string strAutoSave = "AutoSave.xml";

            XmlWriterSettings settings = new XmlWriterSettings();
            settings.ConformanceLevel = ConformanceLevel.Document;
            settings.Indent = true;

            using (XmlWriter xml = XmlWriter.Create(strAutoSave, settings))
            {
                xml.WriteStartElement(m_strVersionNumber);
                xml.WriteStartElement("Map");
                xml.WriteAttributeString("Name", strAutoSave);
                xml.WriteAttributeString("Isometric", m_bIsIsometric.ToString());
                xml.WriteAttributeString("TotalTiles", m_mMap.NTotalNumTiles.ToString());
                xml.WriteAttributeString("Columns", m_mMap.NNumCols.ToString());
                xml.WriteAttributeString("Rows", m_mMap.NNumRows.ToString());
                xml.WriteAttributeString("CellWidth", m_mMap.NCellWidth.ToString());
                xml.WriteAttributeString("CellHeight", m_mMap.NCellHeight.ToString());
                xml.WriteAttributeString("NumTilesets", (tabControl1.TabCount - 1).ToString());
                xml.WriteEndElement();

                for (int i = 0; i < tabControl1.TabCount - 1; ++i)
                {
                    xml.WriteStartElement("Tileset");
                    xml.WriteAttributeString("FileName", m_strTilesetFilenames[i]);
                    xml.WriteAttributeString("ColorKeyRed", m_clrKey[i].R.ToString());
                    xml.WriteAttributeString("ColorKeyGreen", m_clrKey[i].G.ToString());
                    xml.WriteAttributeString("ColorKeyBlue", m_clrKey[i].B.ToString());
                    xml.WriteAttributeString("Width", m_tsTileset[i].NTilesetWidth.ToString());
                    xml.WriteAttributeString("Height", m_tsTileset[i].NTilesetHeight.ToString());
                    xml.WriteAttributeString("CellWidth", m_tsTileset[i].NCellWidth.ToString());
                    xml.WriteAttributeString("CellHeight", m_tsTileset[i].NCellHeight.ToString());
                    xml.WriteEndElement();
                }

                xml.WriteStartElement("Layer1");
                for (int i = 0; i < m_mMap.NTotalNumTiles; ++i)
                {
                    if (m_mMap.TMapTiles[i].NSourceID != -1)
                    {
                        xml.WriteStartElement("TILE");
                        xml.WriteElementString("MapID", i.ToString());
                        xml.WriteElementString("ImageID", m_mMap.TMapTiles[i].ImageID.ToString());
                        xml.WriteElementString("ID", m_mMap.TMapTiles[i].NSourceID.ToString());
                        xml.WriteElementString("Flag", m_mMap.TMapTiles[i].NTileFlag.ToString());
                        xml.WriteElementString("SourceX", m_mMap.TMapTiles[i].SourceRect.X.ToString());
                        xml.WriteElementString("SourceY", m_mMap.TMapTiles[i].SourceRect.Y.ToString());
                        xml.WriteElementString("Width", m_mMap.TMapTiles[i].SourceRect.Width.ToString());
                        xml.WriteElementString("Height", m_mMap.TMapTiles[i].SourceRect.Height.ToString());
                        xml.WriteElementString("Trigger", m_mMap.TMapTiles[i].Trigger);
                        xml.WriteEndElement();
                    }
                }
                xml.WriteEndElement();

                xml.WriteStartElement("Layer2");
                for (int i = 0; i < m_mMap.NTotalNumTiles; ++i)
                {
                    if (m_mMap.TMapTilesLayer2[i].NSourceID != -1)
                    {
                        xml.WriteStartElement("TILE");
                        xml.WriteElementString("MapID", i.ToString());
                        xml.WriteElementString("ImageID", m_mMap.TMapTilesLayer2[i].ImageID.ToString());
                        xml.WriteElementString("ID", m_mMap.TMapTilesLayer2[i].NSourceID.ToString());
                        xml.WriteElementString("Flag", m_mMap.TMapTilesLayer2[i].NTileFlag.ToString());
                        xml.WriteElementString("SourceX", m_mMap.TMapTilesLayer2[i].SourceRect.X.ToString());
                        xml.WriteElementString("SourceY", m_mMap.TMapTilesLayer2[i].SourceRect.Y.ToString());
                        xml.WriteElementString("Width", m_mMap.TMapTilesLayer2[i].SourceRect.Width.ToString());
                        xml.WriteElementString("Height", m_mMap.TMapTilesLayer2[i].SourceRect.Height.ToString());
                        xml.WriteElementString("Trigger", m_mMap.TMapTilesLayer2[i].Trigger.ToString());
                        xml.WriteEndElement();
                    }
                }
                xml.WriteEndElement();

                xml.WriteStartElement("FreePlace");
                for (int i = 0; i < m_mMap.FreePlaced.GetLength(0); ++i)
                {
                    if (m_mMap.FreePlaced[i] != null)
                    {
                        xml.WriteStartElement("FREETILE");
                        xml.WriteElementString("ImageID", m_mMap.FreePlaced[i].ImageID.ToString());
                        xml.WriteElementString("SourceX", m_mMap.FreePlaced[i].SourceRect.X.ToString());
                        xml.WriteElementString("SourceY", m_mMap.FreePlaced[i].SourceRect.Y.ToString());
                        xml.WriteElementString("Flag", m_mMap.FreePlaced[i].NTileFlag.ToString());
                        xml.WriteElementString("X", m_mMap.FreePlaced[i].MapPt.X.ToString());
                        xml.WriteElementString("Y", m_mMap.FreePlaced[i].MapPt.Y.ToString());
                        xml.WriteElementString("Width", m_mMap.FreePlaced[i].SourceRect.Width.ToString());
                        xml.WriteElementString("Height", m_mMap.FreePlaced[i].SourceRect.Height.ToString());
                        xml.WriteElementString("Trigger", m_mMap.FreePlaced[i].Trigger.ToString());

                        xml.WriteEndElement();
                    }
                }
                xml.WriteEndElement();
                xml.WriteEndElement();
            }
            return true;
        }
        private bool SaveBinary(bool bHaveSaved)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            DialogResult dr = new DialogResult();
            sfd.FileName = m_strSaveFileName + ".dat";
            sfd.Filter = "DAT Files|*.dat";
            if (!bHaveSaved)
            {
                dr = sfd.ShowDialog();
            }
            if (dr == DialogResult.Cancel)
                return false;

            if (dr == DialogResult.OK || bHaveSaved)
            {
                m_strSaveFileName = Path.GetFileNameWithoutExtension(sfd.FileName);
                this.Text = "TED - " + m_strSaveFileName;
                string newFileName = Path.GetFileName(sfd.FileName);
                using (BinaryWriter bw = new BinaryWriter(File.Open(newFileName, FileMode.Create)))
                {
                    bw.Write(m_strVersionNumber);
                    //bw.Write(m_bIsIsometric);
                    //bw.Write(/*"FileName=" + */Path.GetFullPath(sfd.FileName) + "\n");
                    bw.Write(m_mMap.NTotalNumTiles);
                    bw.Write(m_mMap.NNumCols);
                    bw.Write(m_mMap.NNumRows);
                    int x = 0, y = 0;
                    string[] tempFileName = new string[MAX_NUM_TILESETS];
                    for (int i = 0; i < tabControl1.TabCount - 1; ++i)
                    {
                        bw.Write("Tileset");
                        tempFileName[i] = Path.GetFileName(m_strTilesetFilenames[i]);
                        if (tempFileName[i][0] != 'V' || tempFileName[i][1] != 'G' || tempFileName[i][2] != '_')
                        {
                            tempFileName[i] = tempFileName[i].Insert(0, "VG_");
                        }
                        bw.Write("Resources/Images/" + tempFileName[i]); // full file path
                        bw.Write(tempFileName[i]); // tileset name, to compare each tile's source tileset
                        bw.Write(m_clrKey[i].R);
                        bw.Write(m_clrKey[i].G);
                        bw.Write(m_clrKey[i].B);
                        //bw.Write("Width=" + m_tsTileset[i].NTilesetWidth);
                        //bw.Write("Height=" + m_tsTileset[i].NTilesetHeight);
                        //bw.Write("CellWidth=" + m_tsTileset[i].NCellWidth);
                        //bw.Write("CellHeight=" + m_tsTileset[i].NCellHeight);
                    }
                    if (m_mMap.ContainsTile[(int)LAYER.LAYER_ONE])
                    {
                        bw.Write("Layer1");
                        for (int i = 0; i < m_mMap.NTotalNumTiles; ++i)
                        {
                            if (m_mMap.TMapTiles != null && m_mMap.TMapTiles[i].NSourceID != -1)
                            {
                                for (int id = 0; id < tabControl1.TabCount - 1; ++id)
                                {
                                    if (m_mMap.TMapTiles[i].ImageID == m_nCurrImageID[id])
                                        bw.Write(tempFileName[id]);
                                }
                                bw.Write(m_mMap.TMapTiles[i].NSourceID);
                                bw.Write(x);
                                bw.Write(y);
                                bw.Write(m_mMap.TMapTiles[i].NTileFlag);
                                bw.Write(m_mMap.TMapTiles[i].SourceRect.Width);
                                bw.Write(m_mMap.TMapTiles[i].SourceRect.Height);
                                bw.Write(m_mMap.TMapTiles[i].Trigger);
                            }
                            x++;
                            if (x == m_mMap.NNumCols)
                            {
                                x = 0;
                                y++;
                            }
                        }
                    }
                    x = y = 0;
                    if (m_bIsTwoLayersOn)
                    {
                        if (m_mMap.ContainsTile[(int)LAYER.LAYER_TWO])
                        {
                            bw.Write("Layer2");
                            for (int i = 0; i < m_mMap.NTotalNumTiles; ++i)
                            {
                                if (m_mMap.TMapTilesLayer2[i].NSourceID != -1)
                                {
                                    for (int id = 0; id < tabControl1.TabCount - 1; ++id)
                                    {
                                        if (m_mMap.TMapTilesLayer2[i].ImageID == m_nCurrImageID[id])
                                            bw.Write(tempFileName[id]);
                                    }
                                    bw.Write(m_mMap.TMapTilesLayer2[i].NSourceID);
                                    bw.Write(x);
                                    bw.Write(y);
                                    bw.Write(m_mMap.TMapTilesLayer2[i].NTileFlag);
                                    bw.Write(m_mMap.TMapTilesLayer2[i].SourceRect.Width);
                                    bw.Write(m_mMap.TMapTilesLayer2[i].SourceRect.Height);
                                    bw.Write(m_mMap.TMapTilesLayer2[i].Trigger);
                                }
                                x++;
                                if (x == m_mMap.NNumCols)
                                {
                                    x = 0;
                                    y++;
                                }
                            }
                        }
                        if (m_mMap.ContainsTile[(int)LAYER.LAYER_FREE])
                        {
                            bw.Write("FreePlace");
                            for (int i = 0; i < m_mMap.FreePlaced.GetLength(0); ++i)
                            {
                                if (m_mMap.FreePlaced[i] != null)
                                {
                                    for (int id = 0; id < tabControl1.TabCount - 1; ++id)
                                    {
                                        if (m_mMap.FreePlaced[i].ImageID == m_nCurrImageID[id])
                                            bw.Write(tempFileName[id]);
                                    }
                                    bw.Write(m_mMap.FreePlaced[i].SourceRect.X);
                                    bw.Write(m_mMap.FreePlaced[i].SourceRect.Y);
                                    bw.Write(m_mMap.FreePlaced[i].NTileFlag);
                                    bw.Write(m_mMap.FreePlaced[i].MapPt.X);
                                    bw.Write(m_mMap.FreePlaced[i].MapPt.Y);
                                    bw.Write(m_mMap.FreePlaced[i].SourceRect.Width);
                                    bw.Write(m_mMap.FreePlaced[i].SourceRect.Height);
                                    bw.Write(m_mMap.FreePlaced[i].Trigger);
                                }
                            }
                        }
                    }
                }
            }            
            m_bChangesMade = false;
            m_bHaveSaved = true;
            return true;
        }
        private void xMLToolStripMenuItem_Click(object sender, EventArgs e)
        {
            loadType = (int)LOAD_TYPE.XML;
            OpenFile();
        }

        private void binaryToolStripMenuItem_Click(object sender, EventArgs e)
        {
            loadType = (int)LOAD_TYPE.BINARY;
            OpenFile();
        }
        /// <summary>
        /// Handles binary, text, and xml loading depending on which was chosen
        /// 
        ///     <LoadParams>
        /// 
        ///     Save Variables:
        ///     Version number.
        ///     Map Name.
        ///     Is it isometric?
        ///     Total number of tiles.
        ///     Columns.
        ///     Rows.
        ///     Cell Width.
        ///     Cell Height.
        ///     "Layer Name".
        ///     
        ///         <IndividualTiles>
        ///         Map ID.
        ///         Image ID.
        ///         ID (tileset tile ID).
        ///         Flag.
        ///         (Free Placed adds x and y, source rect is used for tileset x and y).
        ///         Tile Width.
        ///         Tile Height.
        ///         Trigger.
        ///         </IndividualTiles>
        ///     
        ///     </SaveParams>
        /// </summary>
        /// <param name="fileName">The file to load from</param>
        /// <returns>void</returns>
        private void OpenFile()
        {
            OpenFileDialog ofd = new OpenFileDialog();

            switch (loadType)
            {
                case (int)LOAD_TYPE.XML:
                    {
                        ofd.FileName = ".xml";
                        ofd.Filter = "XML Files|*.xml";
                        if (DialogResult.OK == ofd.ShowDialog(this))
                        {
                            if (Path.GetExtension(ofd.FileName) == ".xml" ||
                                Path.GetExtension(ofd.FileName) == ".XML")
                            {
                                this.Text = "TED - " + Path.GetFileNameWithoutExtension(ofd.FileName);
                                m_mMap = null;
                                for (int i = 0; m_tsTileset[i] != null; ++i)
                                {
                                    m_tsTileset[i] = null;
                                    m_tsComponents[i] = null;
                                    m_strTilesetFilenames[i] = null;
                                    m_mTM.ReleaseTexture(m_nCurrImageID[i]);
                                    m_nCurrImageID[i] = -1;
                                    m_clrKey[i] = Color.White;
                                }
                                XmlReaderSettings settings = new XmlReaderSettings();
                                settings.ConformanceLevel = ConformanceLevel.Document;
                                settings.IgnoreComments = true;
                                settings.IgnoreWhitespace = true;

                                using (XmlReader reader = XmlReader.Create(ofd.FileName, settings))
                                {
                                    reader.MoveToContent();
                                    if (reader.Name == m_strVersionNumber)
                                    {
                                        bool bIsometric;
                                        int nNumTiles;
                                        int nNumCols;
                                        int nNumRows;
                                        int nCellWidth;
                                        int nCellHeight;
                                        int nNumTilesets = 0;
                                        reader.ReadStartElement();
                                        if (reader.Name == "Map")
                                        {
                                            m_strSaveFileName = reader.GetAttribute("Name");

                                            bIsometric   = Convert.ToBoolean(reader.GetAttribute("Isometric"));
                                            nNumTiles    = Convert.ToInt32(reader.GetAttribute("TotalTiles"));
                                            nNumCols     = Convert.ToInt32(reader.GetAttribute("Columns"));
                                            nNumRows     = Convert.ToInt32(reader.GetAttribute("Rows"));
                                            nCellWidth   = Convert.ToInt32(reader.GetAttribute("CellWidth"));
                                            nCellHeight  = Convert.ToInt32(reader.GetAttribute("CellHeight"));
                                            nNumTilesets = Convert.ToInt32(reader.GetAttribute("NumTilesets"));

                                            // set up the map
                                            if (bIsometric)
                                            {
                                                m_mMap = new CMap(nCellWidth, nCellHeight, nNumCols, nNumRows, m_nZoomIncrement, true);
                                                nudMapCellSize.Enabled = false;
                                            }
                                            else
                                            {
                                                m_mMap = new CMap(nCellWidth, nNumCols, nNumRows, m_nZoomIncrement, false);
                                                nudMapCellSize.Value = nCellWidth;
                                                nudMapCellSize.Enabled = true;
                                            }
                                            splitContainer1.Panel2.AutoScrollMinSize = new Size(nNumCols * nCellWidth, nNumRows * nCellHeight);
                                            m_mMap.NPanelWidth = splitContainer1.Panel2.Width;
                                            m_mMap.NPanelHeight = splitContainer1.Panel2.Height;
                                            m_mMap.NZoom = (int)nudMapZoom.Value;
                                            m_mMap.ClrTilesetKey = Color.White;
                                            nudMapNumCols.Value = nNumCols;
                                            nudMapNumRows.Value = nNumRows;
                                        }

                                        m_nCurrTilesetIndex = 0;

                                        // set up all the tilesets
                                        {
                                            string filename;
                                            int imageID;
                                            int red, green, blue;
                                            Color clrKey = Color.White;
                                            int width;
                                            int height;
                                            int cellwidth = 0;
                                            int cellheight = 0;
                                            for (int i = 0; i < tabControl1.TabCount - 1; ++i )
                                            {
                                                tabControl1.TabPages.RemoveAt(i + 1);
                                            }
                                            for (int i = 0; i < nNumTilesets; ++i)
                                            {
                                                reader.ReadStartElement();
                                                if (reader.Name == "Tileset")
                                                {
                                                    filename = reader.GetAttribute("FileName");
                                                    m_strTilesetFilenames[i] = (filename);
                                                    red = Convert.ToInt32(reader.GetAttribute("ColorKeyRed"));
                                                    green = Convert.ToInt32(reader.GetAttribute("ColorKeyGreen"));
                                                    blue = Convert.ToInt32(reader.GetAttribute("ColorKeyBlue"));
                                                    width = Convert.ToInt32(reader.GetAttribute("Width"));
                                                    height = Convert.ToInt32(reader.GetAttribute("Height"));
                                                    cellwidth = Convert.ToInt32(reader.GetAttribute("CellWidth"));
                                                    cellheight = Convert.ToInt32(reader.GetAttribute("CellHeight"));
                                                    clrKey = Color.FromArgb(red, green, blue);
                                                    imageID = m_mTM.LoadTexture((filename), Color.FromArgb(clrKey.R, clrKey.G, clrKey.B).ToArgb());
                                                    m_nCurrImageID[i] = imageID;
                                                    bNewTS = true;
                                                    AddTileset(imageID, clrKey, width, height, cellwidth, cellheight, filename);
                                                    bNewTS = false;
                                                    m_clrKey[i] = clrKey;
                                                }
                                            }
                                            Rectangle SourceRect = new Rectangle(0, 0, cellwidth, cellheight);
                                            m_tCurrTile = new CTILE(0, SourceRect, m_nCurrTileFlag, m_nCurrImageID[m_nCurrTilesetIndex]);
                                        }
                                        // set up the map tiles
                                        int mapID = 0;
                                        int tileImageID = 0;
                                        int sourceID = 0;
                                        int flag = 0;
                                        int tileX = 0;
                                        int tileY = 0;
                                        int tileWidth = 0;
                                        int tileHeight = 0;
                                        string trigger = "";
                                        Rectangle srcRect;
                                        m_mMap.NCurrLayer = (int)LAYER.LAYER_ONE;
                                        reader.ReadToFollowing("Layer1");

                                        reader.Read();
                                        while (reader.Name == "TILE")
                                        {
                                            reader.Read();
                                            mapID = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            tileImageID = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            sourceID = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            flag = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            tileX = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            tileY = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            tileWidth = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            tileHeight = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            trigger = reader.ReadString();

                                            srcRect = new Rectangle(tileX, tileY, tileWidth, tileHeight);
                                            CTILE tempTile = new CTILE(sourceID, srcRect, flag, tileImageID);
                                            tempTile.Trigger = trigger;

                                            m_mMap.AddTile(tempTile, mapID);
                                            reader.Read();
                                            reader.Read();
                                        }
                                        m_mMap.NCurrLayer = (int)LAYER.LAYER_TWO;
                                        reader.ReadToFollowing("Layer2");
                                        reader.Read();
                                        while (reader.Name == "TILE")
                                        {
                                            reader.Read();
                                            mapID = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            tileImageID = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            sourceID = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            flag = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            tileX = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            tileY = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            tileWidth = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            tileHeight = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            trigger = reader.ReadString();

                                            srcRect = new Rectangle(tileX, tileY, tileWidth, tileHeight);
                                            CTILE tempTile = new CTILE(sourceID, srcRect, flag, tileImageID);
                                            tempTile.Trigger = trigger;

                                            m_mMap.AddTile(tempTile, mapID);
                                            reader.Read();
                                            reader.Read();
                                        }
                                        m_mMap.NCurrLayer = (int)LAYER.LAYER_FREE;
                                        if (reader.Name != "FreePlace")
                                        {
	                                        reader.ReadToFollowing("FreePlace");
                                        }
                                        reader.Read();
                                        int posX, posY, count = 0;
                                        while (reader.Name == "FREETILE")
                                        {
                                            reader.Read();
                                            tileImageID = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            tileX = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            tileY = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            flag = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            posX = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            posY = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            tileWidth = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            tileHeight = Convert.ToInt32(reader.ReadString());
                                            reader.Read();
                                            trigger = reader.ReadString();

                                            srcRect = new Rectangle(tileX, tileY, tileWidth, tileHeight);
                                            CFREETILE tempFree = new CFREETILE(posX, posY, srcRect, flag, tileImageID);
                                            tempFree.Trigger = trigger;
                                            // TODO:: save and load rotation
                                            m_mMap.LoadFreePlacedTiles(tempFree, count++);
                                            reader.Read();
                                            reader.Read();
                                        }
                                    }
                                }
                                return;
                            }
                            else // try again
                            {
                                MessageBox.Show("You have entered an invalid file extension.",
                                                "Failed to load", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                OpenFile();
                            }
                        }
                    }
                    break;
                case (int)LOAD_TYPE.BINARY:
                    {
                        ofd.FileName = ".dat";
                        ofd.Filter = "DATA Files|*.dat";
                        if (DialogResult.OK == ofd.ShowDialog(this))
                        {
                            if (Path.GetExtension(ofd.FileName) == ".dat" ||
                                Path.GetExtension(ofd.FileName) == ".DAT")
                            {
                                m_mMap = null;
                                for (int i = 0; i < MAX_NUM_TILESETS; ++i)
                                {
                                    this.Text = "TED - " + Path.GetFileNameWithoutExtension(ofd.FileName);
                                    m_tsTileset[i] = null;
                                    m_tsComponents[i] = null;
                                    m_strTilesetFilenames[i] = null;
                                    m_nCurrImageID[i] = -1;
                                    m_clrKey[i] = Color.White;
                                }
                                return;
                            }
                            else // try again
                            {
                                MessageBox.Show("You have entered an invalid file extension.",
                                                "Failed to load", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                OpenFile();
                            }
                        }
                    }
                    break;
            }
        }
    }

}