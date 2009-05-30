﻿using System;
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
        string m_strTrigger;
        public string Trigger
        {
            get { return m_strTrigger; }
            set { m_strTrigger = value; }
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
        }
        public CTILE(int sourceID, Rectangle srcRect, int tileFlag, int imageID)
        {
            m_nSourceID = sourceID;
            SourceRect = srcRect;
            m_nTileFlag = tileFlag;
            m_nImageID = imageID;
            m_nPOIX = 0;
            m_nPOIY = 0;
            m_strTrigger = "None";
        }
    }
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
            m_strTrigger = "None";
        }
        public CFREETILE(int PosX, int PosY, Rectangle srcRect, int tileFlag, int imageID)
        {
            SourceRect = srcRect;
            m_nTileFlag = tileFlag;
            m_nImageID = imageID;
            m_ptMapPt.X = PosX;
            m_ptMapPt.Y = PosY;
            m_nAnchorX = 0;
            m_nAnchorY = 0;
            m_strTrigger = "None";
        }
    }
}