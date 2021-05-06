using Windows.UI.Xaml.Controls;

namespace Lab5
{
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        private void Hub_SectionHeaderClick(object sender, HubSectionHeaderClickEventArgs e)
        {
            switch (e.Section.Name)
            {
                case "About":
                    this.Frame.Navigate(typeof(About));
                    break;
                case "Conv":
                   this.Frame.Navigate(typeof(Convert));
                    break;
                default:
                    this.Frame.Navigate(typeof(About));
                    break;

            }
        }
    }
    
}
