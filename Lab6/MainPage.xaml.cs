using System;
using System.Collections.Generic;
using Windows.Storage;
using Windows.Storage.FileProperties;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Input;

namespace Lab6
{

    public sealed partial class MainPage : Page
    {
        private IReadOnlyList<IStorageItem> itemsList;
        private StorageFolder currentFolder;

        public MainPage()
        {
            this.InitializeComponent();
        }

        private async void ButtonBase_OnClick(object sender, RoutedEventArgs e)
        {
            Windows.Storage.Pickers.FolderPicker folderPicker = new Windows.Storage.Pickers.FolderPicker();
            folderPicker.ViewMode = Windows.Storage.Pickers.PickerViewMode.Thumbnail;
            folderPicker.FileTypeFilter.Add(".txt");

            StorageFolder folder = await folderPicker.PickSingleFolderAsync();
            ScanDir(folder);
        }

        private async void ScanDir(StorageFolder folder)
        {
            if (folder != null)
            {
                listBox.Items.Clear();

                currentFolder = folder;

                StorageFolder parentFolder = await folder.GetParentAsync();
                ListBoxItem lbItem = new ListBoxItem();
                lbItem.DoubleTapped += Navigate;
                lbItem.Tag = parentFolder;
                listBox.Items.Add(lbItem);

                itemsList = await folder.GetItemsAsync();
                foreach (var item in itemsList)
                {
                    lbItem = new ListBoxItem();
                    lbItem.Tag = item;
                    if (item is StorageFolder)
                    {
                        lbItem.Content =  item.Name + "\tдата створення: " + item.DateCreated.DateTime;
                    }
                    else
                    {
                        BasicProperties properties = await item.GetBasicPropertiesAsync();
                        ulong size = properties.Size;
                        string sizestr = "B";

                        if (size > 1024)
                        {
                            sizestr = "KB";
                            size /= 1024;
                        }

                        if (size > 1024)
                        {
                            sizestr = "MB";
                            size /= 1024;
                        }

                        if (size > 1024)
                        {
                            sizestr = "GB";
                            size /= 1024;
                        }

                        lbItem.Content = item.Name + "\tдата створення: " + item.DateCreated.DateTime + "\tрозмір: " + size + sizestr;
                    }

                    listBox.Items.Add(lbItem);
                    lbItem.DoubleTapped += Navigate;
                }

            }
        }

        private void Navigate(object sender, DoubleTappedRoutedEventArgs e)
        {
            ListBoxItem lbItem = (ListBoxItem)sender;

            if (lbItem.Tag is StorageFolder)
            {
                ScanDir((StorageFolder)lbItem.Tag);
            }

        }

        private async void DeleteFile_OnClick(object sender, RoutedEventArgs e)
        {
            if (listBox.SelectedItem == null)
                return;

            ListBoxItem item = (ListBoxItem)listBox.SelectedItem;
            if (item.Tag is Windows.Storage.StorageFile)
            {
                Windows.Storage.StorageFile file = (Windows.Storage.StorageFile)item.Tag;
                await file.DeleteAsync();
            }
            ScanDir(currentFolder);
        }

        private async void CopyFile_OnClick(object sender, RoutedEventArgs e)
        {
            if (listBox.SelectedItem == null)
                return;

            ListBoxItem item = (ListBoxItem)listBox.SelectedItem;

            if (item.Tag is Windows.Storage.StorageFile)
            {
                Windows.Storage.StorageFile file = (Windows.Storage.StorageFile)item.Tag;
                await file.CopyAsync(currentFolder, fileName.Text);
            }
            ScanDir(currentFolder);
        }
    }

}

