//  [5/27/2009 Ramon Johannessen (RJ)]

/// <summary>
///  File : Help.cs
///  Purpose : All the help text that populates the help window
/// </summary>
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace test
{
    /// <summary>
    /// 
    /// </summary>
    public partial class Help : Form
    {
        public Help()
        {
            InitializeComponent();
        }

        private void Help_Load(object sender, EventArgs e)
        {
            rtbGeneral.Text = "To begin making a map...\n\n" +

                "Click File->New Map.\n\n" +
                "If you desire to make an isometric map, check the Isometric box. In this case, you will " +
                "have to set both the cell width and height.\n" +
                "You must set values for number of columns and number of rows.\n" +
                "The cell size sets the width and height of the tiles unless you choose isometric.\n" +
                "In which case you must set the width and height of the cells.\n\n" +

                "You may use the existing default image as a tileset, but it is recommended\n" +
                "that you load in your own. You may replace an existing tileset.\n\n" +
                "To do so Click File->Import->Tileset(Replace).\n\n" +
                "Click the Import Image button. You may load in any common image file type.\n" +
                "Then select the size of the tileset's tiles, both width and height.\n" +
                "You may also set the transparency color in case you want to utilize the second or free place layers.\n" +
                "To do so, click the key color button (after loading the file) and click the square and drag to the color " +
                "you wish to select as the key color.\n" +
                "When you are done, press the Create Tileset button.\n" +
                "The status bar in the left corner will tell you if the image has loaded successfully\n" +
                "If you wish to simply add a new tileset instead of replacing, click File->Import->Tileset(Add).\n\n" +

                "Now you may begin using your tileset to create maps.\n\n" +
                "If you wish to utilize the second layer simply press SPACEBAR" +
                " or, under the Map tab page, change the" +
                "Current Layer to Layer 2....it will automatically switch your Current Layer Mode to both layers.\n" +
                "You may change this at any time by pressing Q(Layer1), W(Layer2), or E(both)...OR by going in the " +
                "Map tab page under Current Layer Mode and selecting an option from the drop down list.\n\n" +

                "You may also select more than one tile from the tileset by dragging the mouse across as many tiles" +
                "as you wish to select in a rectangle at once. When you place the tiles, they will always place" +
                "from the top left down and to the right. If you happen to place tiles off the right edge of the map" +
                "they will end up drawing in the first columns of the next row...so beware.\n\n" +

                "You may change the background color of the map and the tileset through the options menu.\n\n" +

                "To delete tiles that you have placed on the map, right-click on the tile you wish to delete. You may also " +
                "right-click and drag to continuously delete tiles.\n\n";

            rtbHotKeys.Text = "All Hotkeys, including the menu bar...\n\n" +
                "Ctrl + N \t--    Brings up the Create New Map dialog box.\n" +
                "Ctrl + I \t--    Brings up the Replace Tileset dialog box.\n" +
                "Ctrl + A \t--    Brings up the Add Tileset dialog box.\n" +
                "Ctrl + 0 \t--    Brings up the Open Saved Map dialog box.\n" +
                "Ctrl + B \t--    Brings up the Save to Binary dialog box.\n" +
                "Ctrl + T \t--    Brings up the Save to Text dialog box.\n" +
                "Ctrl + X \t--    Brings up the Save to XML dialog box.\n" +

                "Ctrl + F \t--    Brings up the Set Tile Flags dialog box.\n" +
                "Shift+ R-click\t Brings up the Set Trigger String dialog box.\n\n" +

                "0 (zero) \t--    sets currently selected tile(s) flag to 0\n" +
                "1 (one)  \t--    sets currently selected tile(s) flag to 1\n" +
                "2        \t--    sets currently selected tile(s) flag to 2\n" +
                "3        \t--    sets currently selected tile(s) flag to 3\n\n" +

                "Q        \t--    set the current view to Layer 1 only.\n" +
                "W        \t--    set the current view to Layer 2 only.\n" +
                "E        \t--    set the current view to both first and second layers.\n" +
                "R        \t--    set the current view to all layers.\n\n" +

                "F        \t--    fills all tiles of Current Layer with the selected tile.\n" +
                "C        \t--    clears all tiles of Current Layer.\n" +

                "S        \t--    toggle on/off (S)how tile flags.\n" +
                "G        \t--    toggle on/off (G)rid lines.\n" +
                "SPACE    \t--    toggles between Currently Selected layers.\n";

        }
        
    }
}
