use tokio::runtime::Runtime;
use thiserror::Error;


#[derive(Error, Debug)]
pub enum BeerError {
    #[error("the requested beer does not exist")]
    NotFound(),
    #[error(transparent)]
    Network(#[from] reqwest::Error)
}


#[cxx::bridge]
mod ffi  {
    #[derive(Deserialize, Debug)]
    struct Beer {
        name: String,
        description: String,
        id: u32,
        abv: f32
    }


    extern "Rust" {
        fn get_beers() -> Result<Vec<Beer>>;
        fn get_beer_by_id(id: u32) -> Result<Beer>;
        fn get_beer_by_name(name: &str) -> Result<Vec<Beer>>;
    }
}

pub fn get_beers() -> Result<Vec<ffi::Beer>, BeerError> {
    let rt = Runtime::new().expect("Could not start tokio runtime");

    let result = rt.block_on(async {
        let request_url = String::from("https://api.punkapi.com/v2/beers");
        println!("Making request to {}", request_url);
        let response = reqwest::get(&request_url).await?;

        response.error_for_status_ref()?;

        let beers: Vec<ffi::Beer> = response.json().await?;
        Ok(beers)
    });

    result
}

pub fn get_beer_by_id(id: u32) -> Result<ffi::Beer, BeerError> {
    let rt = Runtime::new().expect("Could not start tokio runtime");

    let result = rt.block_on(async {
        let request_url = format!("https://api.punkapi.com/v2/beers/{id}");
        println!("Making request to {}", request_url);
        let response = reqwest::get(&request_url).await?;

        response.error_for_status_ref()?;

        let beers: Vec<ffi::Beer> = response.json().await?;
        let result = beers.into_iter().nth(0).ok_or(BeerError::NotFound());

        result
    });

    result
}

pub fn get_beer_by_name(name: &str) -> Result<Vec<ffi::Beer>, BeerError> {
    let rt = Runtime::new().expect("Could not start tokio runtime");

    let result = rt.block_on(async {
        let request_url = format!("https://api.punkapi.com/v2/beers?beer_name={}", name);
        println!("Making request to {}", request_url);
        let response = reqwest::get(&request_url).await?;

        response.error_for_status_ref()?;

        let beers: Vec<ffi::Beer> = response.json().await?;
        println!("Size of beers: {}", beers.len());

        if beers.is_empty() {
            return Err(BeerError::NotFound())
        }

        Ok(beers)
    });

    result
}