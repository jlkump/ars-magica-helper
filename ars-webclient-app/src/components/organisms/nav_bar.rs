use stylist::yew::styled_component;
use yew::prelude::*;
use yew_icons::{Icon, IconId};

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
}

#[styled_component(NavBar)]
pub fn nav_bar(_props: &Props) -> Html {
    html! {
        <span class="navbar">
            <ul>
                <li>
                    <Icon icon_id={IconId::LucideMenu} />
                </li>
                <li>
                    <h1 class="prevent-select">{"Ars Magica Helper"}</h1>
                </li>
                <li class="right margin-small"><Icon icon_id={IconId::BootstrapGithub} /></li>
            </ul>
        </span>
    }
}