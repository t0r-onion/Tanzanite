# Tanzanite Web Server

<img src="tanzanite-run/assets/tanzanite.png" alt="Tanzanite Logo" width="200">

Tanzanite Web Server is a super secure and fast web server developed in C++. It is designed to efficiently serve web content while prioritizing security and speed. With Tanzanite, you can host your websites and web applications with confidence, knowing that your server is optimized for performance and robust security.

## Features

- **High Performance**: Tanzanite is built with efficiency in mind, ensuring lightning-fast response times even under heavy loads.
- **Security**: Security is paramount. Tanzanite incorporates robust security measures to protect against common web vulnerabilities.
- **Ease of Use**: Simple setup and configuration make Tanzanite accessible to developers of all levels.
- **Scalability**: Tanzanite is designed to scale effortlessly as your web traffic grows, ensuring seamless performance.

## Getting Started

To get started with Tanzanite, follow these steps:

1. **Clone the Repository**: Clone this repository to your local machine using the following command:
   ```
   git clone https://github.com/t0r-onion/Tanzanite.git
   ```

2. **Build Tanzanite**: Navigate to the `tanzanite-run` directory and build Tanzanite using CMake:
 Certainly! Here's the updated build instructions for both Linux and Windows:

### Linux

1. **Clone the Repository**: Clone this repository to your local machine using the following command:
   ```
   git clone https://github.com/t0r-onion/Tanzanite.git
   ```

2. **Build Tanzanite**: Navigate to the `tanzanite-run` directory and build Tanzanite using CMake:
   ```
   cd Tanzanite/tanzanite-run
   mkdir build && cd build
   cmake ..
   make
   ```

3. **Run Tanzanite**: Once built, you can run Tanzanite with the following command:
   ```
   ./tanzanite
   ```

### Windows

1. **Clone the Repository**: Clone this repository to your local machine using the following command:
   ```
   git clone https://github.com/t0r-onion/Tanzanite.git
   ```

2. **Build Tanzanite**: Navigate to the `tanzanite-run` directory and build Tanzanite using CMake and MinGW:
   ```
   cd Tanzanite\tanzanite-run
   mkdir build && cd build
   cmake -G "MinGW Makefiles" ..
   mingw32-make
   ```
3. **Run Tanzanite**: Once built, you can run Tanzanite with the following command:
   ```
   Tanzanite.exe
   ```

5. **Access Your Server**: Tanzanite will now be running locally. You can access it by navigating to `http://localhost:8080` in your web browser.

## Contributing

Contributions to Tanzanite are welcome! If you have any ideas for new features, bug fixes, or improvements, feel free to open an issue or submit a pull request.

## License

Tanzanite is licensed under the [MIT License](LICENSE).
