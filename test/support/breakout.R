library(BreakoutDetection)

x <- c(
  3.0, 1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 2.0, 2.0, 3.0,
  6.0, 4.0, 4.0, 5.0, 6.0, 4.0, 4.0, 4.0, 6.0, 5.0,
  9.0, 8.0, 7.0, 9.0, 8.0, 9.0, 9.0, 9.0, 7.0, 9.0
)

print("multi")
breakout(x, min.size=5, method="multi")

print("percent")
breakout(x, min.size=5, method="multi", percent=0.5)

print("amoc")
breakout(x, min.size=5, method="amoc")

print("tail")
breakout(x, min.size=5, method="amoc", exact=FALSE)

print("simple")
x <- c(
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
)
breakout(x, min.size=5, method="multi")

print("constant")
x <- rep(1, 100)
breakout(x, method="amoc")
