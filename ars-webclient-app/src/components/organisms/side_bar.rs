use yew::prelude::*;
use yew_icons::{Icon, IconId};

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
}

#[function_component(SideBar)]
pub fn side_bar(_props: &Props) -> Html {
    html! {
        <div class="sidebar">
            <ul>
                <li>
                    <Icon icon_id={IconId::FontAwesomeSolidHatWizard} /> <h2 class="prevent-select">{"Item 1"}</h2>
                </li>
                <li>
                    <Icon icon_id={IconId::BootstrapGearFill} />
                </li>
                <li>
                    <Icon icon_id={IconId::FontAwesomeSolidFeather} />
                </li>
                <li>
                    <Icon icon_id={IconId::FontAwesomeSolidPencil} />
                </li>
                <li>
                    <Icon icon_id={IconId::BootstrapTagsFill} />
                </li>
                <li>
                    <Icon icon_id={IconId::FontAwesomeSolidTag} />
                </li>
                <li>
                    <Icon icon_id={IconId::HeroiconsSolidMap} />
                </li>
                <li>
                    <Icon icon_id={IconId::HeroiconsSolidSparkles} />
                </li>
                <li>
                    <Icon icon_id={IconId::FontAwesomeSolidBook} />
                </li>
                <li>
                    <Icon icon_id={IconId::FontAwesomeSolidEye} />
                </li>
                <li>
                    <Icon icon_id={IconId::FontAwesomeSolidEyeLowVision} />
                </li>
                <li>
                    <Icon icon_id={IconId::FontAwesomeSolidHammer} />
                </li>
                <li>
                    <Icon icon_id={IconId::LucideCoins} />
                </li>
                <li>
                    <Icon icon_id={IconId::FontAwesomeSolidSignsPost} />
                </li>
                <li>
                    <Icon icon_id={IconId::BootstrapFileTextFill} />
                </li>
                <li>
                    <Icon icon_id={IconId::BootstrapFileEarmarkImageFill} />
                </li>
                <li>
                    <Icon icon_id={IconId::LucideUpload} />
                </li>
                <li>
                    <Icon icon_id={IconId::LucideDownload} />
                </li>
                <li>
                    <Icon icon_id={IconId::OcticonsX16} />
                </li>
                <li>
                    <Icon icon_id={IconId::FontAwesomeSolidTimeline} />
                </li>
                <li>
                    <Icon icon_id={IconId::FontAwesomeSolidTimeline} />
                </li>
                <li><div></div></li>
            </ul>
        </div>
    }
}