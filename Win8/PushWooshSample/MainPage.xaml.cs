using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using PushSDK;
using System.Text;
using Windows.UI.Popups;
using Windows.UI.StartScreen;
using Windows.UI.Notifications;
using Windows.Data.Xml.Dom;
using Windows.Networking.PushNotifications;
using System.Diagnostics;
// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

namespace PushWooshSample
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private NotificationService service;

        public MainPage()
        {
            this.InitializeComponent();
        }

        void service_OnPushAccepted(object sender, Windows.Networking.PushNotifications.PushNotificationReceivedEventArgs e)
        {
            String notificationContent = String.Empty;

            String type = String.Empty;
            switch (e.NotificationType)
            {
                case PushNotificationType.Badge:
                    notificationContent = e.BadgeNotification.Content.GetXml();
                    type = "Badge";
                    break;

                case PushNotificationType.Tile:

                    notificationContent = e.TileNotification.Content.GetXml();
                    type = "Tile";
                    break;

                case PushNotificationType.Toast:

                    notificationContent = e.ToastNotification.Content.GetXml();
                    type = "Toast";
                    break;

                case PushNotificationType.Raw:
                    notificationContent = e.RawNotification.Content;
                    type = "Raw";
                    break;
            }

            Debug.WriteLine("Received {0} notification", type);
            Debug.WriteLine("Notification content: " + notificationContent);

            var alert = new MessageDialog("Notification content: " + notificationContent, type + " received");
            alert.ShowAsync();
        }

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.  The Parameter
        /// property is typically used to configure the page.</param>
        protected override async void OnNavigatedTo(NavigationEventArgs e)
        {

        }


        private void CheckBox_Checked(object sender, RoutedEventArgs e)
        {
            if (service != null)
            {
                service.StartGeoLocation();
            }

        }

        private void CheckBox_Unchecked(object sender, RoutedEventArgs e)
        {
            if (service != null)
            {
                service.StopGeoLocation();
            }
        }

        private void UnSubButton_Click(object sender, RoutedEventArgs e)
        {
            if (service != null)
            {
                service.UnsubscribeFromPushes();
            }
            SubButton.IsEnabled = true; ;
            UnSubButton.IsEnabled = false;
            tbPushToken.Text = "";
        }

        private void btnSendTag_Click(object sender, RoutedEventArgs e)
        {
            object value;

            if (tbTagValue.Text.IndexOf(',') != -1)
                value = tbTagValue.Text.Replace(", ", ",").Split(',');
            else
                value = tbTagValue.Text;

            object[] Values = new object[] { value };
            string[] Keys = new string[] { tbTagTitle.Text };

            if (service != null)
            {
                service.SendTag(Keys, Values);
            }
        }

        private void DisplaySkippedTags(IEnumerable<KeyValuePair<string, string>> skippedTags)
        {
            StringBuilder builder = new StringBuilder();
            builder.AppendLine("These tags has been ignored:");

            foreach (var tag in skippedTags)
            {
                builder.AppendLine(string.Format("{0} : {1}", tag.Key, tag.Value));
            }
        }


        private void Subscribe_Tapped(object sender_, TappedRoutedEventArgs e)
        {
            try
            {

                string _PWId = PWID.Text;
                service = PushSDK.NotificationService.GetCurrent(_PWId, "", null);
                if (Host.Text.EndsWith("/"))
                {
                    service.SetHost(Host.Text);
                }
                else
                {
                    service.SetHost(Host.Text + "/");
                }

                service.OnPushAccepted += service_OnPushAccepted;
                service.SubscribeToPushService();

                if (service.PushToken != null)
                {
                    tbPushToken.Text = service.PushToken;
                }

                SubButton.IsEnabled = false;
                UnSubButton.IsEnabled = true;
            }
            catch (Exception ex)
            {
                MessageDialog dialog = new MessageDialog("Host does not exist: \n" + ex.Message);
                dialog.ShowAsync();
            }

        }


    }
}
