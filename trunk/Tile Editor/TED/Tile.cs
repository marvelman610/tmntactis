//  [5/27/2009 Ramon Johannessen (RJ)]

/// <summary>
///  File : Tile.cs
///  Purpose : Defines what a tile is, used by the tileset and the map
/// </summary>
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace tile
{
    /// <summary>
    /// 
    /// </summary>
    public class CTILE
    {
        int m_nSourceID;
        public int NSourceID
        {
            get { return m_nSourceID; }
            set { m_nSourceID = value; }
        }
        Rectangle m_sourceRect;
        public System.Drawing.Rectangle SourceRect
        {
            get { return m_sourceRect; }
            set { m_sourceRect = value; }
        }
        int m_nTileFlag;
        public int NTileFlag
        {
            get { return m_nTileFlag; }
            set { m_nTileFlag = value; }
        }
        int m_nImageID;
        public int ImageID
        {
            get { return m_nImageID; }
            set { m_nImageID = value; }
        }
        int m_nPOIX;
        public int AnchorX
        {
            get { return m_nPOIX; }
            set { m_nPOIX = value; }
        }
        int m_nPOIY;
        public int AnchorY
        {
            get { return m_nPOIY; }
            set { m_nPOIY = value; }
        }
        int m_nCost;
        public int Cost
        {
            get { return m_nCost; }
            set { m_nCost = value; }
        }
        string m_strTrigger;
        public string Trigger
        {
            get { return m_strTrigger; }
            set { m_strTrigger = value; }
        }
        string m_strFileName;
        public string FileName
        {
            get { return m_strFileName; }
            set { m_strFileName = value; }
        }
       
        // constructors
        public CTILE()
        {
            m_sourceRect = new Rectangle();
            m_nSourceID = -1;
            m_nTileFlag = 0;
            m_nImageID = -1;
            m_nPOIX = 0;
            m_nPOIY = 0;
            m_strTrigger = "None";
            m_strFileName = "";
            m_nCost = 0;
        }
        public CTILE(int sourceID, Rectangle srcRect, int tileFlag, int imageID, int cost, string trigger, string fileName)
        {
            m_nSourceID = sourceID;
            SourceRect = srcRect;
            m_nTileFlag = tileFlag;
            m_nImageID = imageID;
            m_nCost = cost;
            m_strTrigger = trigger;
            m_nPOIX = 0;
            m_nPOIY = 0;
            m_strFileName = fileName;
        }
        public CTILE (CTILE tile)
        {
            CTILE newTILE = new CTILE();
            this.AnchorX = tile.AnchorX;
            this.AnchorY = tile.AnchorY;
            this.ImageID = tile.ImageID;
            this.NSourceID = tile.NSourceID;
            this.NTileFlag = tile.NTileFlag;
            this.SourceRect = tile.SourceRect;
            this.Trigger = tile.Trigger;
            this.m_nCost = tile.Cost;
            this.FileName = tile.FileName;
        }
    }
    /// <summary>
    /// Free tiles allow the user to place a tile of any origin (custom size, not restricted to the grid)
    /// from a tileset to anywhere on the map.
    /// </summary>
    public class CFREETILE
    {
        Rectangle m_sourceRect;
        public System.Drawing.Rectangle SourceRect
        {
            get { return m_sourceRect; }
            set { m_sourceRect = value; }
        }
        int m_nTileFlag;
        public int NTileFlag
        {
            get { return m_nTileFlag; }
            set { m_nTileFlag = value; }
        }
        int m_nImageID;
        public int ImageID
        {
            get { return m_nImageID; }
            set { m_nImageID = value; }
        }
        Point m_ptMapPt;
        public Point MapPt
        {
            get { return m_ptMapPt; }
            set { m_ptMapPt = value; }
        }
        int m_nAnchorX;
        public int AnchorX
        {
            get { return m_nAnchorX; }
            set { m_nAnchorX = value; }
        }
        int m_nAnchorY;
        public int AnchorY
        {
            get { return m_nAnchorY; }
            set { m_nAnchorY = value; }
        }
        string m_strTrigger;
        public string Trigger
        {
            get { return m_strTrigger; }
            set { m_strTrigger = value; }
        }
        float m_fRotation;
        public float Rotation
        {
            get { return m_fRotation; }
            set { m_fRotation = value; }
        }
        Point m_ptRotationCenter;
        public Point RotationCenter
        {
            get { return m_ptRotationCenter; }
            set { m_ptRotationCenter = value; }
        }
        float m_fScaleX;
        public float ScaleX
        {
            get { return m_fScaleX; }
            set { m_fScaleX = value; }
        }
        float m_fScaleY;
        public float ScaleY
        {
            get { return m_fScaleY; }
            set { m_fScaleY = value; }
        }
        string m_strFilename;
        public string Filename
        {
            get { return m_strFilename; }
            set { m_strFilename = value; }
        }

        // constructors
        public CFREETILE()
        {
            m_sourceRect = new Rectangle();
            m_nTileFlag = 0;
            m_nImageID = -1;
            m_ptMapPt.X = 0;
            m_ptMapPt.Y = 0;
            m_nAnchorX = 0;
            m_nAnchorY = 0;
            m_fScaleX = 1.0f;
            m_fScaleY = 1.0f;
            m_fRotation = 0.0f;
            m_strTrigger = "None";
            m_strFilename = "";
        }
        public CFREETILE(int PosX, int PosY, Rectangle srcRect, int tileFlag, int imageID, float rotation, string fileName)
        {
            SourceRect = srcRect;
            m_nTileFlag = tileFlag;
            m_nImageID = imageID;
            m_ptMapPt.X = PosX;
            m_ptMapPt.Y = PosY;
            m_nAnchorX = 0;
            m_nAnchorY = 0;
            m_fScaleX = 1.0f;
            m_fScaleY = 1.0f;
            m_fRotation = rotation;
            m_strTrigger = "None";
            m_strFilename = fileName;
        }
        public CFREETILE(int PosX, int PosY, Rectangle srcRect, int tileFlag, int imageID, string fileName)
        {
            SourceRect = srcRect;
            m_nTileFlag = tileFlag;
            m_nImageID = imageID;
            m_ptMapPt.X = PosX;
            m_ptMapPt.Y = PosY;
            m_nAnchorX = 0;
            m_nAnchorY = 0;
            m_fScaleX = 1.0f;
            m_fScaleY = 1.0f;
            m_fRotation = 0.0f;
            m_strTrigger = "None";
            m_strFilename = fileName;
        }
    }
}
