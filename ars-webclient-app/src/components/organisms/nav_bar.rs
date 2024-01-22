use stylist::yew::styled_component;
use yew::prelude::*;
use yew_icons::{Icon, IconId};

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    #[prop_or_default]
    pub menu_clicked: Callback<()>
}

#[styled_component(NavBar)]
pub fn nav_bar(props: &Props) -> Html {
    let onclick = props.menu_clicked.reform(|_: MouseEvent| ());
    html! {
        <span class="navbar">
            <ul>
                <li>
                    <a style="cursor: pointer" {onclick}><Icon icon_id={IconId::LucideMenu}/></a>
                </li>
                <li>
                    <h1 class="prevent-select">{"Ars Magica Helper"}</h1>
                </li>
                <li class="right margin-small"><Icon icon_id={IconId::BootstrapGithub} /></li>
            </ul>
        </span>
    }
}